#ifndef __IMAGE_PROCESSING_H__
#define __IMAGE_PROCESSING_H__

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/time.h>

/* function declarations */
int* read_image (const char *file_name, int *p_h, int *p_w, int *p_levels);
void write_image (const int *image, const char *file_name, int h, int w, int levels);

#endif // __IMAGE_PROCESSING_H__