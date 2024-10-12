/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 10/12/24
 * Description: Encodes a secret message using the least significant bits in each pixel
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  	if (argc != 2) {
    		printf("usage: decode <file.ppm>\n");
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
        
        char* msg = malloc(total * 8 + 1);
        unsigned char temp; 
        unsigned char mask = 0b1; 
        unsigned char bit;

        for(int i = 0; i < w * h * 3; i++){
                bit = pixelInfo[i] & mask;
                temp = (temp << 1) | bit; 

                if(i % 8 == 7){
                        msg[i/8] = temp;
                        if(temp == '\0'){
                                break;
                        }
                        temp = 0;
                }
        }

        printf("%s", msg);
        free(msg);
        free(pixels);
  	return 0;
}

