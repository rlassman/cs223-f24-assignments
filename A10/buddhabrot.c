/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 11/15/24
 * Description: Makes a buddhabrot and stores it in a PPM file
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
#include <math.h>
#include <bits/pthreadtypes.h>
#define numProcesses 4

int maxCount = 0;
pthread_mutex_t mutex;
pthread_barrier_t mybarrier;

struct thread_data {
  int row_start;
  int row_end;
  int col_start;
  int col_end;
  struct ppm_pixel** array;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIt;
  pthread_t tid;
  int **membership;
  int **count;
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

  //step 1
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
        data->membership[i][j] = 0;
      } else {
        data->membership[i][j] = 1;
      }
    }
  }

  //step 2
  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = data->col_start; j < data->col_end; j++) {
      if (data->membership[i][j] == 0) {
        float xfrac = i / (float)size;
        float yfrac = j / (float)size;
        float x0 = xmin + xfrac * (xmax - xmin);
        float y0 = ymin + yfrac * (ymax - ymin);

        float x = 0;
        float y = 0;

        while (x*x + y*y < 2*2) {
          float xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;
          int yrow = round(size * (y - ymin)/(ymax - ymin));
          int xcol = round(size * (x - xmin)/(xmax - xmin));
          if (yrow < 0 || yrow >= size) continue; // out of range
          if (xcol < 0 || xcol >= size) continue; // out of range

          pthread_mutex_lock(&mutex);
          data->count[yrow][xcol] ++;
          if (data->count[yrow][xcol] > maxCount) {
            maxCount = data->count[yrow][xcol];
          }
          pthread_mutex_unlock(&mutex);
        }
      }
    }
  }
  
 pthread_barrier_wait(&mybarrier);

  //step 3
  float gamma = 0.681;
  float factor = 1.0/gamma;
  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = data->col_start; j < data->col_end; j++) {
      float value = 0;
      if (data->count[i][j] > 0) {
        value = log(data->count[i][j]) / log(maxCount);
        value = pow(value, factor);
      }

      struct ppm_pixel color;
      color.red = value * 225;
      color.green = value * 225;
      color.blue = value * 225;

      data->array[i][j] = color;
    }
  }

  printf("Thread %lu finished\n", (unsigned long)data->tid);
 
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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image

  srand(time(0));

  // compute image
  struct ppm_pixel** pixels = malloc(sizeof(struct ppm_pixel *) * size);
  for(int i=0; i<size; i++) {
        pixels[i] = malloc(sizeof(struct ppm_pixel) * size);
  }

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&mybarrier,NULL,numProcesses);

  pthread_t threads[numProcesses];
  struct thread_data data[numProcesses];
  int subsize = size/2;

  int **membership = malloc(sizeof(int *) * size);
  for(int i=0; i<size; i++) {
        membership[i] = malloc(sizeof(int) * size);
  }

  int **count = malloc(sizeof(int *) * (size));
  for(int i=0; i<size; i++) {
        count[i] = malloc(sizeof(int) * (size));
  }
  for(int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      count[i][j] = 0;
    }
  }

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
    data[i]. size  = size;
    data[i].xmin = xmin; 
    data[i].xmax = xmax; 
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIt = maxIterations;
    data[i].count = count;
    data[i].membership = membership;
    pthread_create(&threads[i], NULL, fill_ppm_array, (void*) &data[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%dx%d) in %lf seconds\n", size, size, timer);

  char file[100];
  sprintf(file, "buddhabrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", file);
  write_ppm_2d(file, pixels, size, size);

  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&mybarrier);

  for(int i=0; i<size; i++) {
		free(membership[i]);
	}
  for(int i=0; i<size; i++) {
		free(count[i]);
	}
  free(membership);
  free(count);

  for(int i=0; i<size; i++) {
		free(pixels[i]);
	}
  free(pixels);
}