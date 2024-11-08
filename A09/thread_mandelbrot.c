/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 11/8/24
 * Description: Generates an image in a PPM file from a mandelbrot set using threading
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#define numProcesses 4

struct thread_data {
  int row_start;
  int row_end;
  int col_start;
  int col_end;
  struct ppm_pixel** array;
  struct ppm_pixel* palette;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIt;
  pthread_t tid;
};

void *fill_ppm_array(void *userdata) {
  struct thread_data *data = (struct thread_data *) userdata;
  data->tid = pthread_self();
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n", 
        (unsigned long)data->tid, data->col_start, data->col_end, data->row_start, data->row_end);
  
  int size = data->size;
  float xmin = data->xmin;
  float xmax = data->xmax;
  float ymin = data->ymin;
  float ymax = data->ymax;
  int maxIterations = data->maxIt;
  
  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = data->col_start; j < data->col_end; j++) {
      float xfrac = i / (float)size;
      float yfrac = j / (float)size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < maxIterations && x*x + y*y < 2*2) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      if (iter < maxIterations) {// escaped
        data->array[j][i] = data->palette[iter];
      } else {
        data->array[j][i].red = 0;
        data->array[j][i].green = 0;
        data->array[j][i].blue = 0;
      }
    }
  }
  return (void*) NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  //int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate pallet
  srand(time(0));

  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  for(int i=0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // compute image
  struct ppm_pixel** pixels = malloc(sizeof(struct ppm_pixel *) * size);
  for(int i=0; i<size; i++) {
        pixels[i] = malloc(sizeof(struct ppm_pixel) * size);
  }

  pthread_t threads[numProcesses];
  struct thread_data data[numProcesses];
  int subsize = size/2;

  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  for (int i = 0; i < numProcesses; i++) {
    int row_block = i/2; 
    int col_block = i % 2; 
    data[i].row_start = row_block * subsize;
    data[i].row_end = (row_block + 1) * subsize;
    data[i].col_start = col_block * subsize;
    data[i].col_end = (col_block + 1) * subsize;

    data[i].array = pixels;
    data[i].palette = palette; 
    data[i]. size  = size;
    data[i].xmin = xmin; 
    data[i].xmax = xmax; 
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIt = maxIterations;
    pthread_create(&threads[i], NULL, fill_ppm_array, (void*) &data[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < numProcesses; i++) {
    printf("Thread %lu finished\n", (unsigned long)data[i].tid);
  }

  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %lf seconds\n", size, size, timer);

  char file[100];
  sprintf(file, "mandelbrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", file);
  write_ppm_2d(file, pixels, size, size);

  for(int i=0; i<size; i++) {
		free(pixels[i]);
	}
  free(pixels);
  free(palette);
}
