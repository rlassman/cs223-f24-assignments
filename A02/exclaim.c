#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char symbols[4] = {'@', '!', '#', '*'};
  int symbMin = 0;
  int symbMax = 3; 
  char buff[32];
  
  printf("Enter a word: ");
  scanf(" %s", buff);

  int i = 0;
  while(i <= strlen(buff)) { //string length
	int charval = buff[i];
	if (charval > 96)
		buff[i] = symbols[rand() % (symbMax - symbMin + 1) + symbMin];
	i++;
  }

  printf("OMG! %s\n", buff);
  return 0;
}
