/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 9/20/24
 * Description: Prints characteristics for a list of songs
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct song {
	char title[128];
  	char artist[128];
  	int durMin;
	int durSec;
  	float dance;
	float energy;
	float tempo;
	float valence;
} song;

int printSongs(song* songArr, int n) {
	for(int i=0; i<n; i++) {
        	printf("\n%d) %-30s artist: %-20s duration: %d:%02d  danceability: %.2f energy:  %.2f tempo:  %.2f valence:  %.2f", 
                       i, songArr[i].title, songArr[i].artist, songArr[i].durMin, songArr[i].durSec, songArr[i].dance, songArr[i].energy, 
		       songArr[i].tempo, songArr[i].valence);
  	}
  	printf("\n\n========================\n"); 
	return 0;
}

int main() {
	FILE *infile ;
	infile = fopen("songlist.csv", "r");
	if (infile == NULL) {
		printf("Error, unable to open file");
		exit(1);
	}
 	
	char buffer[100];
	fgets(buffer, 100, infile); 
	char *tok = strtok(buffer, ",");
	int size = atoi(tok);

	song* songs = malloc(sizeof(song) * size);
	fgets(buffer, 100, infile);
	for (int i = 0; i < size; i++) {
		fgets(buffer, 100, infile);
		tok = strtok(buffer, ",");
		strcpy(songs[i].title, tok);
		tok = strtok(NULL, ",");
		strcpy(songs[i].artist, tok);
		tok = strtok(NULL, ",");
		int dur = atoi(tok)/1000;
		songs[i].durMin = dur/60;
		songs[i].durSec = dur % 60;
		tok = strtok(NULL, ",");
		songs[i].dance = atof(tok);
		tok = strtok(NULL, ",");
                songs[i].energy = atof(tok);
		tok = strtok(NULL, ",");
                songs[i].tempo = atof(tok);
		tok = strtok(NULL, ",");
                songs[i].valence = atof(tok);
	}

	printSongs(songs, size);	
	free(songs);
	fclose(infile);
	return 0;
}
