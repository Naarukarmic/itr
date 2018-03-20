#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#include <stdio.h>
#include <time.h>

#include "image_processing.h"

#define MAX_CALCULATION_NUMBER 1000

// La matrice Kernel utilisee pour la convolution
const int kernel[3][3] = {{-1 , 0 , 1} , {-3, 0 , 3} , {-1 , 0 , 1} };

/* 
 * Compute the resulting image after doing convolution of the submatrix
 * sub_image_in of size (h+2) * w with kernel. the result is and array 
 * of size h * w
 */
int* compute_sub_image_out(const int *sub_image_in , int h, int w);

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  char nom_process[128];
  int nb_process , identifiant , taille_nom;
  MPI_Comm_size(MPI_COMM_WORLD, &nb_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &identifiant);
  MPI_Get_processor_name(nom_process, &taille_nom);

  if (argc != 4 ){
    printf("Wrong number of arguments ! \n");
    printf(" image_conv image_in_path image_out_path 0|1(no stats|stats of the  operations) \n");
    exit(-1);
  }

  char *IMAGE_IN  = argv[1];
  char *IMAGE_OUT = argv[2];
  int compute_statistics = atoi(argv[3]);

  printf("image_in : %s ; image_out : %s; stats : %d \n", IMAGE_IN , IMAGE_OUT , compute_statistics);
  double *time_stats = NULL;

  int i, h, w, levels ;

  struct timespec spec;
  double init_time, end_time;

  int *image_in = NULL;
  int *image_out = NULL;

  int *sendcounts = NULL;
  int *displs_send = NULL;

  /* Number of lines to perform by this process + number of width             */
  int nb_lines , width;

  int nb_calcul = compute_statistics ? 0 : (MAX_CALCULATION_NUMBER-1);

while(nb_calcul < MAX_CALCULATION_NUMBER){

  /* Read the image, divide the image' total number of lines by the number of 
   * process and (for the moment) send them the size of the sub_matrix to handle
   */ 
  if (identifiant == 0){
    /* read image from a file --> See modfied version of read_image
     * which does dynamic allocation
     */
    if (image_in == NULL){
      image_in = read_image(IMAGE_IN , &h , &w, &levels);
      /* When statistcis for time analysis need to be done , do it            */
      if (compute_statistics)
        time_stats = (double *) malloc(sizeof(double) * MAX_CALCULATION_NUMBER);
    }

    /* allocate image to be return                                             */
    image_out = (int *) malloc( sizeof(int) * h *w);

    /* Start counting time here */
    clock_gettime(CLOCK_MONOTONIC_RAW, &spec);
    init_time = ((double) (spec.tv_nsec / 1.0e6)) + (double) (spec.tv_sec * 1.0e3);

    /* array specifying the number of elements to send/receive to/from each processor*/
    sendcounts = (int *) malloc(sizeof(int) * nb_process);

    /*Index i specifies the displacement (relative to sendbuf) from which to take the 
     * outgoing data to process */
    displs_send = (int *) malloc(sizeof(int) * nb_process);

    /* Since 3 rows won't change due to border consideration, we consider only h-3 rows
     * to share amongst processes*/
    int nb_elem_to_handle = (h-3) / nb_process;
    int nb_elem_remain = (h-3) % nb_process;

    /* We add 2 because of the convolution formula where the line before the current
     * line and the line after the current line are needed -> 2 sup lines */
    nb_lines = nb_elem_to_handle + 2;
    width = w;

    sendcounts[0] = nb_lines * w;
    displs_send[0] = 0; // Normally should be 1 but we start 1 line before as explained above
    //printf("Process 0 : Rows from %d to %d\n", displs_send[0]/w , displs_send[0]/w + sendcounts[0]/w);
    
    for (i=1 ; i< nb_process ; i++){
      /* If the number of elements isn't divisible by the number of process, we add
       * 1 more lines to each process until there is no remaining */
      sendcounts[i] = 2 + nb_elem_to_handle + (nb_elem_remain>0 ? 1 : 0);
      /* Ensures that for every chunks we start on the appropriate index */
      displs_send[i] = displs_send[i-1] + sendcounts[i-1] - 2*w;
      nb_elem_remain-= 1;
      /* After knowing the size for each process , we send it to them */
      int dim_data[2] = {sendcounts[i] , w};
      MPI_Send(dim_data , 2 , MPI_INT , i ,0 , MPI_COMM_WORLD);
      sendcounts[i] *= w;
      // printf("Process %d : Rows from %d to %d\n", i , displs_send[i]/w , displs_send[i]/w + sendcounts[i]/w);
    }

    /* process 0 fill lines 0 , lines h-1 and h-2 since they are not handled by other processes*/
    for(i=0 ; i< h ; i++){
      image_out[i] = image_in[i];
      image_out[(h-2) *w + i] = image_in[(h-2) *w + i];
      image_out[(h-1) *w + i] = image_in[(h-1) *w + i];
    } 
  }

  /* Non root processes must receive information about the size of elements they will handle*/
  if(identifiant != 0){
    int dim_data[2]; 
    MPI_Recv(dim_data , 2 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
    nb_lines = dim_data[0];
    width = dim_data[1];
  }

  // printf("Hello %s, id : %d , size data : %d * %d \n", nom_process, identifiant,nb_lines,width);

  /* Here root process sends each chunks to the appropriate pprocess (including him) and
   * also here non-root process are able to receive the sub matrix and store them in sub_image*/
  int *sub_image_in = (int * ) malloc(sizeof(int) * nb_lines * width);
  MPI_Scatterv(image_in , sendcounts , displs_send , MPI_INT  , sub_image_in , nb_lines*width, MPI_INT , 0 , MPI_COMM_WORLD);

  /* we compute the sub_image_out to be sent back to the root process
   * Note that sub_image_out has 2 lines less than sub_image_in for the reasons above*/
  int *sub_image_out = compute_sub_image_out(sub_image_in , nb_lines-2 , width);

  /* Before sending back the computed array, update size and index according to image_out*/
  if (identifiant == 0){
    for(i=1 ; i< nb_process ; i++){
      displs_send[i] += 1;
      sendcounts[i] -= 2*w;
    }
  }
  /* Send back the computed data to the root proces */
  MPI_Gatherv(sub_image_out,(nb_lines-2)*width,MPI_INT,image_out,sendcounts,displs_send,MPI_INT,0,MPI_COMM_WORLD);

  /* Write the image obtained by merging all the data from differents processes*/
  if(identifiant == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &spec);
    double end_time = ((double) (spec.tv_nsec / 1.0e6)) + (double) (spec.tv_sec * 1.0e3);
    if(compute_statistics){
      time_stats[nb_calcul] = end_time - init_time; 
    }else{
      write_image(image_out , IMAGE_OUT , h, w, levels);
      printf("Elapsed time : %f\n", end_time-init_time);   
    }
  }

  /* Start freeing allocated variable */
  free(sub_image_in);
  sub_image_in = NULL;
  free(sub_image_out);
  sub_image_out = NULL;

  if(identifiant == 0){
    free(image_out);
    free(displs_send);
    free(sendcounts);
    image_out = NULL;
    displs_send = NULL;
    sendcounts = NULL;
  }
  nb_calcul++;
}

  if(identifiant == 0){
    if (compute_statistics){
      char data_out[15];
      sprintf(data_out , "calcul%d.txt", nb_process);
      FILE *fin;
      fin = fopen (data_out, "w");
      for(i = 0 ; i < MAX_CALCULATION_NUMBER-1 ; i++){
        fprintf (fin, "%f\n", time_stats[i]); 
      }
      fprintf (fin, "%f", time_stats[MAX_CALCULATION_NUMBER-1]); 
      fclose(fin);
    }
    free(image_in);
    free(time_stats);
  }

  printf("Goodbye  %s, id %d \n", nom_process, identifiant);
  MPI_Finalize();
}

int* compute_sub_image_out(const int *sub_image_in , int h_im_out, int w_im_out){
  int *sub_image_out = (int *) malloc(sizeof(int) * h_im_out * w_im_out);
  int i , j , k , l;
  for (i=0 ; i < h_im_out ; i++){
    for( j=0 ; j < w_im_out ; j++){
      if (j == 0 || j>= w_im_out-2 ){
        sub_image_out[i*w_im_out + j] = sub_image_in[(i+1)*w_im_out + j];
      }else{
        sub_image_out[i*w_im_out + j] = 0;
        for( k = 0 ; k <=2 ; k++ ){
          for( l = 0 ; l<=2 ; l++){
            sub_image_out[i*w_im_out + j] += kernel[k][l] * sub_image_in[(i+k)*w_im_out + j+ l -1]; 
          }
        }
        if (sub_image_out[i*w_im_out + j] < 0)
          sub_image_out[i*w_im_out + j] = 0;
        if (sub_image_out[i*w_im_out + j]>255)
          sub_image_out[i*w_im_out + j] = 255;
      }
    }
  }
  return sub_image_out;  
}