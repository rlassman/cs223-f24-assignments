/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 10/4/24
 * Description: Writes a ppm file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

	FILE *outfile;
	outfile = fopen(filename, "w");
	if(!outfile) {
		printf("Unable to open file.");
		return;
	}

	fprintf(outfile, "P6\nCreated by Rebecca\n%d %d \n255\n", w, h);
	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			unsigned char pix[3] = {pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue};
			fwrite(pix, sizeof(unsigned char), 3, outfile);
		}
	}
	fclose(outfile);
}
