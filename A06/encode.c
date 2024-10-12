/* Author: Rebecca Lassman
 * Date: 10/12/24
 * Description: Decodes a secret message from the least significant bits in each pixel
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  	if (argc != 2) {
    		printf("usage: encode <file.ppm>\n");
    		return 0;
  	}

	int w, h;
	struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
	if (pixels == NULL) {
    		printf("Error: Could not read file %s\n", argv[1]);
   		 return 0;
    	}
  	unsigned char* pixelInfo = (unsigned char*)pixels; 
	int total = ((w * h) * 3)/8; 
	printf("Reading %s with width %d and height %d \n", argv[1], w, h);
  	printf("Max number of characters in the image: %d\n", total);
	
	char *phrase = malloc(sizeof(char)*total);
  	printf("Enter a phrase:\n");
  	scanf("%[^\n]", phrase);
  	unsigned char newColor;
  	unsigned char mask = 0b1; 
  	unsigned char bit;

	for(int i = 0; i < w * h * 3; i++){
    		bit = pixelInfo[i] & mask;

    		if(i >= strlen(phrase) * 8){
      			bit = 0;
    		} else{
     			bit = (phrase[i/8] >> (7 - i % 8)) & 0b1;
    		}
    		newColor = (pixelInfo[i] & ~mask) | bit;
    		pixelInfo[i] = newColor; 
  	}

	char outputFile[256];
  	strcpy(outputFile, argv[1]); 
  	char *dot = strrchr(outputFile, '.');
  	if (dot != NULL) {
    		strcpy(dot, "-encoded.ppm"); 
  	} 
  	else {
    		strcat(outputFile, "-encoded.ppm");
  	}
  	write_ppm(outputFile, pixels, w, h);
  	printf("Writing file %s \n", outputFile);

  	free(phrase);
 	free(pixels);
  	return 0;
}


