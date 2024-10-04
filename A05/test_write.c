/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 10/4/24
 * Description Tests writing a ppm file
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  	int w, h;
  	struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

	write_ppm_2d("test.ppm", pixels, w, h);
	struct ppm_pixel** pixels2 = read_ppm_2d("test.ppm", &w, &h);
	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			printf("(%u, %u, %u) ", pixels2[i][j].red, pixels2[i][j].green, pixels2[i][j].blue);
		}
		printf("\n");
	}

	for(int i=0; i<h; i++) {
		free(pixels[i]);
	}
	for(int i=0; i<h; i++) {
                free(pixels2[i]);
        }
  	free(pixels);
	free(pixels2);
 	 return 0;
}
