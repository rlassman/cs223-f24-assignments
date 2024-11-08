/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 11/8/24
 * Description: Generates an image in a PPM file from a mandelbrot set
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);
  // todo: your work here
  // generate pallet
  srand(time(0));

  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  for(int i=0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  struct ppm_pixel** pixels = malloc(sizeof(struct ppm_pixel *) * size);
  for(int i=0; i<size; i++) {
        pixels[i] = malloc(sizeof(struct ppm_pixel) * size);
  }


  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  for (int i=0; i<size; i++) {
   for (int j=0; j<size; j++) {
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
        pixels[j][i] = palette[iter];
      } else {
        pixels[j][i].red = 0;
        pixels[j][i].green = 0;
        pixels[j][i].blue = 0;
      }
    }
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
