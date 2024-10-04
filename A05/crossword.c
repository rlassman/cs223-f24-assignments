/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 10/1/24
 * Description: Creates a crossword puzzle with two given words
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Usage: %s <word1> <word2>\n", argv[0]);
		exit(0);
  	}

  	int l1 = strlen(argv[1]);
	int l2 = strlen(argv[2]);
	char *word1 = malloc(sizeof(char) * (l1 + 1));
	char *word2 = malloc(sizeof(char) * (l2 + 1));
	strcpy(word1, argv[1]);
	strcpy(word2, argv[2]);
	int word1In = -1;
	int word2In = -1;
	
	for(int i=0; i<l1; i++) {
		for(int j=0; j<l2; j++) {
			if (word1[i] == word2[j]) {
				word1In = i;
				word2In = j;
				break;
			}
		}
		if (word1In != -1)
			break;
	}
	if (word1In == -1) {
		printf("No common letter\n");
		free(word1);
		free(word2);
		exit(1);
	}

	char **matrix;
	matrix = malloc(sizeof(char *) * (l1 * l2));
	for(int i=0; i<l1; i++) {
		matrix[i] = malloc(sizeof(char) * l2);
	}

	for(int i=0; i<l1; i++) {
		int j;
		for(j=0; j<l2; j++) {
			if (j == word2In) {
				matrix[i][j] = word1[i];
			} else if (i == word1In) {
				matrix[i][j] = word2[j];
			}
			else {
				matrix[i][j] = '.';
			}
		}
	}

	for(int i=0; i<l1; i++) {
		int j;
		for (j=0; j<l2; j++) {
			printf(" %c ", matrix[i][j]);
		}
		printf("\n");
	}

	for(int i=0; i<l1; i++) {
		free(matrix[i]);
	}

	free(word1);
	free(word2);
	free(matrix);	
  	return 0;
}

