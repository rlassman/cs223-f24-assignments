/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 9/20/24
 * Description: Converts a phrase to entish
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int pause = 0;
	char phrase[32];
	char *entish;
	int entIn = 0;
	int length;

	printf("Pause length: ");
	scanf("%d", &pause);
	printf("Text: ");
	scanf(" %[^\n]%*c", phrase);
	length = strlen(phrase);
	entish = malloc(sizeof(char) * (length * pause + length + 1));

	for (int i = 0; i < strlen(phrase); i++) {
		entish[entIn] = phrase[i];
		entIn++;
		for (int j = 0; j < pause; j++) {
			entish[entIn] = '.';
			entIn ++;
		}	
	}
	entish[entIn] = '\0';
	printf("%s\n", entish);

	free(entish);
	return 0;
}
