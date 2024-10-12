/*----------------------------------------------
 * Author: Rebecca  Lassman
 * Date: 10/12/24
 * Description: Creates an 8x8 1 bpp sprite
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  	unsigned long img;
  	scanf(" %lx", &img);
  	printf("Image (unsigned long): %lx\n", img);

	for(int i=1; i<=64; i++) {
		unsigned long mask = 0x1ul << (63 - i + 1);
		unsigned long bit = (img & mask);

		if (bit) {
			printf("@");
		} else {
			printf(" ");
		}

		if (i % 8 == 0) {
			printf("\n");
		}
	}
  	return 0;
}
