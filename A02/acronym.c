/***************************************************
 * acronym.c
 * Author: Rebecca Lassman
 * Creates an acronym from user input.
 */

#include <stdio.h>
#include <string.h>

int main() {
	char phrase[1024];
	char acr[1024];

	printf("Enter a phrase: ");
	scanf(" %[^\n]%*c", phrase);

	int acrEnd = 0;
	for (int i = 0; i < strlen(phrase); i++) {
		int letter = phrase[i];
		if (letter > 64 && letter < 91) {
			acr[acrEnd] = phrase[i];
			acrEnd ++;
		}
	}
	acr[acrEnd] = '\0';
	printf("Your acronym is %s\n", acr);
	return 0;
}
