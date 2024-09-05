/***************************************************
 * mathgame.c
 * Author: Rebecca Lassman
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {

  int response = 0;
  int correct = 0;
  int max = 9;
  int min = 1;

  printf("Welcome to Math Game!\nHow many rounds do you want to play? ");
  scanf("%d", &response);
  
  for (int i=0; i<response; i++) {
	int num1 = rand() % (max - min + 1) + min;
	int num2 = rand() % (max - min + 1) + min;
	int answer = 0;
	printf("\n%d + %d = ? ", num1, num2);
	scanf("%d", &answer);
	if (answer == num1 + num2) {
		printf("Correct!\n");
		correct ++;
	} else
		printf("Incorrect :(\n");
  }

  printf("You answered %d/%d correctly.\n", correct, response);

  
}

