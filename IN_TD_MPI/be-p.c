/* 
  a simple example of image processing: copy an initial image to an output image
  with the help of 2 functions read_image and write_image
  begin
    read the initial image from a file and copy the pixels in an array (image_in)
    copy the elements of this array to a second array (image_out)
    (this step will be replaced by a more elaborated processing)
    copy the elements of the second array in a file
  end

  the format of the files is the pgm format (alias P2, ASCII file that a standard text editor car process)
  the images are in black and white
  this format is understood by a lot of image readers (eog, display, gimp, ...)
  the (graphical) display of the images is done outside this program

  the pixel arrays are static arrays, defined in the header of the file (there is more place
  in the heap than in the stack (an obvious improvement is to use dynamy memory allocation (malloc)
  the functions are fast coded, the numbers of parameters is minimal but the program works

  constants are defined H (height), W (width), IMAGE_IN (name of the input file), IMAGE_OUT (output file)
  you have to change these values if you want to change the image to process

  to obtain the executable code:
  gcc -Wall -std=c99 image_processing_example.c -o image_processing
  to execute:
  ./image_processing
  to display:
  eog logo2.pgm

*/
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
void main_img_sendto(int rank, int image_in[H][W], int n_lines, int size);
void main_img_recv(int rank, int image_out[H][W], int n_lines, int size);


int main(int argc, char **argv)
{
  int i, j, h, w, levels ;
  struct timeval tdeb, tfin;

  MPI_Init(&argc,&argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* image processing */
  gettimeofday(&tdeb, NULL);
  
  /* Image division */
  int n_lines = H / size;
  
  if(rank == 0) { 
    read_image (image_in, IMAGE_IN, &h, &w, &levels);

    for(i = 1; i < size; i++) {
      main_img_sendto(i, image_in, n_lines, size);
    }
    printf("0 - all sent done\n");

    for(i = 1; i < size; i++) {
      main_img_recv(i, image_out, n_lines, size);
    }
    printf("0 - all recv done\n");

  } else {
    int stripe[n_lines + 2][W];
    int res[n_lines + 2][W];

    printf("%d - recv\n", rank);
    if(rank == size - 1) {
      MPI_Recv(&stripe, W * (H - rank * (n_lines + 1)), 
        MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      MPI_Recv(&stripe, W * (n_lines + 2), 
        MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    printf("%d - recv done\n", rank);

    for (i = 1; i < n_lines; i++) {
      for (j = 1; j < W - 1; j++) {
        res[i][j] = -1 * stripe[i-1][j-1] + 1*stripe[i-1][j+1];
        res[i][j] += -3 * stripe[i][j-1] + 3*stripe[i][j+1];
        res[i][j] += -1 * stripe[i+1][j-1] + 1*stripe[i+1][j+1];
        if(res[i][j] < 0) {
          res[i][j] = 0;
        } else if (res[i][j] > 255) {
          res[i][j] = 255;
        }
      }
    }
    printf("%d - calc done\n", rank);
    if(rank == size - 1) {
      MPI_Send(&res + W, W * (H - (rank-1) * n_lines), MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
      MPI_Send(&res + W, W * n_lines, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    printf("%d - sent done\n", rank);

    
  }

  gettimeofday(&tfin, NULL);
  printf ("computation time (microseconds): %ld\n",  (tfin.tv_sec - tdeb.tv_sec)*1000000 + (tfin.tv_usec - tdeb.tv_usec));

  MPI_Finalize();
  write_image (image_out, IMAGE_OUT, h, w, levels);

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

void main_img_sendto(int rank, int image_in[H][W], int n_lines, int size) {
  printf("0 - Send to %d\n", rank);
  if (rank == 1) {

    MPI_Send(&image_in,
      W * (n_lines + 2), 
      MPI_INT, rank, 0, MPI_COMM_WORLD);

  } else if (rank == size -1) {

    MPI_Send(&image_in + (rank-1) * (n_lines + 1), 
      W * (H - rank * (n_lines + 1)), 
      MPI_INT, rank, 0, MPI_COMM_WORLD);

  } else {

    MPI_Send(&image_in + (rank-1) * (n_lines + 1),
      W * (n_lines + 2), 
      MPI_INT, rank, 0, MPI_COMM_WORLD);

  }
  printf("0 - Sent to %d\n", rank);
  return;
}

void main_img_recv(int rank, int image_out[H][W], int n_lines, int size) {
  printf("0 - Recv from %d\n", rank);
  if (rank == 1) {

    MPI_Recv(&image_out,
      W * n_lines,
      MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  } else if (rank == size -1) {

    MPI_Recv(&image_out + (rank - 1) * n_lines, 
      W * (H - (rank-1) * n_lines),
      MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  } else {

    MPI_Recv(&image_out + (rank - 1) * n_lines,
      W * n_lines,
      MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  }
  printf("0 - Recv to %d\n", rank);
  return;
}
