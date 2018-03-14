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


int main(int argc, char **argv)
{

  int i, j, h, w, levels ;
  struct timeval tdeb, tfin;

  read_image (image_in, IMAGE_IN, &h, &w, &levels); 

  MPI_Init(&argc,&argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* image processing */
  gettimeofday(&tdeb, NULL);
  
  /* Copy */
  for (i = 0; i < h ; i++) {
    for (j = 0; j < w; j++) {
      image_out[i][j] = image_in[i][j]; 
    }
  }
  

  for (i = 1; i < h-1 ; i++) {
    for (j = 1; j < w-1; j++) {
      int point[8] = {0};
      /* point : [i, j, (i-1,j-1), (i-1,j+1),
                (i,j-1), (i, j+1), (i+1,j-1), (i+1,j+1)] */

      if(rank == 0) { 
        point[0] = i;
        point[1] = j;
        point[2] = image_in[i-1][j-1];
        point[3] = image_in[i-1][j+1];
        point[4] = image_in[i][j-1];
        point[5] = image_in[i][j+1];
        point[6] = image_in[i+1][j-1];
        point[7] = image_in[i+1][j+1];

      } else {
        int res[3];
        MPI_Recv(&point, 8, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        res[0] = point[0];
        res[1] = point[1];
        res[2] = -1 * point[2] + 1*point[3];
        res[2] += -3 * point[4] + 3*point[5];
        res[2] += -1 * point[6] + 1*point[7];
        if(res[2] < 0) {
          res[2] = 0;
        } else if (res[2] > 255) {
          res[2] = 255;
        }
        MPI_Send(&res, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }

    }
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
