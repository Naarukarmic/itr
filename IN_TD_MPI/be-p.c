#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/time.h> 
#include <mpi.h>
#define W 943
#define H 827
#define IMAGE_IN "logo.pgm"
#define IMAGE_OUT "logo2.pgm"

/*  global variables */
int image_in[H][W];   
int image_out[H][W];

/* function declarations */
void read_image (int image[H][W], char file_name[], int *p_h, int *p_w, int *p_levels);
void write_image (int image[H][W], char file_name[], int h, int w, int levels);


int main(int argc, char **argv)
{

  MPI_Init ( &argc, &argv);
  
  int rank;
  int size; //nb process
  
  int nb_line, id;
  int i, j, h, w, k, l, levels ;
    struct timeval tdeb, tfin;
    
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  
  if(rank == 0){// master reads and send picture & parameters
      read_image (image_in, IMAGE_IN, &h, &w, &levels); 
      gettimeofday(&tdeb, NULL);
      int nb_line = (int) h / size; // cast to make the compiler happy
      int id;
      // sending params
      for (id = 1; id < size; id++) {
        MPI_Send(&nb_line, 1, MPI_INT, id, 0, MPI_COMM_WORLD);
        MPI_Send(&w, 1, MPI_INT, id, 0, MPI_COMM_WORLD);
      }
      // sending picture chunks
      for (id = 1; id < size; id++) {
        int ligne;
        sleep(2); // fix for timing related issues
        for (ligne = (id - 1) * nb_line; ligne < (id * nb_line) + 1; ligne++){
          MPI_Send(image_in[ligne], w, MPI_INT, id, 0, MPI_COMM_WORLD);
        }
      }
      // receiving processed chunks
      for (id = 1; id < size; id++) {
        int ligne;
        for (ligne = (id - 1) * nb_line; ligne < id * nb_line + 1; ligne++){
          MPI_Recv(image_out[ligne], w, MPI_INT, id, 0, MPI_COMM_WORLD, &status);
        }
    }
    gettimeofday(&tfin, NULL);
    printf ("computation time (microseconds): %ld\n",  (tfin.tv_sec - tdeb.tv_sec)*1000000 + (tfin.tv_usec - tdeb.tv_usec));

    write_image(image_out, IMAGE_OUT, h, w, levels);
  }  
    
  if (rank != 0){ // workers receiving params
    MPI_Recv(&nb_line, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&w, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    int ligne, i, j, k, l;
    for(ligne = (rank - 1) * nb_line; ligne < rank * nb_line; ligne++){
      // workers receiving chunk
      MPI_Recv(image_in[ligne], w, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);   
    }
    for (i = (rank - 1) * nb_line; i < rank * nb_line + 1 ; i++) { // chunk processing
      for (j = 0; j < w; j++) {
        if ( i==0 || i == h - 1 || j == 0 || j == w - 1){ //edge cases
          image_out[i][j] = image_in[i][j];
        }
        else{ // filtering
          image_out[i][j] = -1 * image_in[i-1][j-1] + 1*image_in[i-1][j+1];
          image_out[i][j] += -3 * image_in[i][j-1] + 3*image_in[i][j+1];
          image_out[i][j] += -1 * image_in[i+1][j-1] + 1*image_in[i+1][j+1];
          
          if(image_out[i][j] < 0){ // trimming excess values
            image_out[i][j] = 0;
          }
          if(image_out[i][j] > 255){
            image_out[i][j] = 255;
          }
        }
      }
    }
    for(ligne = (rank - 1) * nb_line; ligne < rank * nb_line + 1; ligne++){ 
      MPI_Send(image_out[ligne], w, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
  
  MPI_Finalize();
  return 0;
}

  
  

void read_image (int image[H][W], char file_name[], int *p_h, int *p_w, int *p_levels){
  FILE *fin;
  int i, j, h, w, levels ;
  char buffer[80];

  /* open P2 image */
  fin = fopen (IMAGE_IN, "r");
  if (fin == NULL){
    printf ("file open error\n");
    exit(-1);
  } 
  fscanf (fin, "%s", buffer);
  if (strcmp (buffer, "P2")){
    printf ("the image format must be P2\n");
    exit(-1);
  }
  fgets (buffer, 80, fin);
  fgets (buffer, 80, fin);
  fscanf (fin, "%d%d", &w, &h);
  fscanf (fin, "%d", &levels);
  printf ("image reading ... h = %d w = %d\n", h, w);

  /* pixels reading */
  for (i = 0; i < h ; i++)
    for (j = 0; j < w; j++)
       fscanf (fin, "%d", &image[i][j]); 
  fclose (fin);

  *p_h = h;
  *p_w = w;
  *p_levels = levels;
  return;

}
void write_image (int image[H][W], char file_name[], int h, int w, int levels){
  FILE *fout;
  int i, j;

  /* open the file */
  fout=fopen(IMAGE_OUT,"w");
  if (fout == NULL){
    printf ("file opening error\n");
    exit(-1);
  }
  
  /* header write */
  fprintf(fout,"P2\n# test \n%d %d\n%d\n", w, h, levels);
  /* format P2, commentary, w x h points, levels */

  /* pixels writing*/
  for (i = 0; i < h ; i++)
    for (j = 0; j < w; j++)
       fprintf (fout, "%d\n", image[i][j]); 

  fclose (fout);
  return;
}

