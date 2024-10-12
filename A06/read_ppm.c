/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 10/12/24
 * Description: Reads a ppm file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
	FILE *infile ;
	infile = fopen(filename, "r");
	if (infile == NULL) {
		printf("Error, unable to open file");
		return NULL;
	}

	char ppm[4];
	fgets(ppm, 4, infile);
	char ppm2[100];
	fgets(ppm2, 100, infile);
	int max;
	fscanf(infile, "%d %d\n%d\n", w, h, &max);

	struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * (*w * *h));

	for(int i=0; i<*w; i++) {
		for(int j=0; j<*h; j++) {
			unsigned char ch = fgetc(infile);
			struct ppm_pixel nPix;
			nPix.red = ch; 
			ch = fgetc(infile);
			nPix.green = ch;
			ch = fgetc(infile);
			nPix.blue = ch;
			pixels[i * (*w) + j] = nPix;
		}
	}

  	fclose(infile);
  	return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  	return NULL;
}
