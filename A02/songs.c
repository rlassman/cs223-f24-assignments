/***************************************************
 * songs.c
 * Author: Rebecca Lassman
 * Prints songs and their attributes, and allows user to edit attributes.
 */

#include <stdio.h>
#include <string.h>

typedef struct song {
	char title[32];
  	char artist[32];
  	int durMin;
  	int durSec;
  	float dance;
} song;

int printSongs(song songArr[]) {
	for(int i=0; i<3; i++) {
        	printf("\n%d) %-20s artist: %-20s duration: %d:%d  danceability: %.2f", 
                       i, songArr[i].title, songArr[i].artist, songArr[i].durMin, songArr[i].durSec, songArr[i].dance);
  	}
  	printf("\n\n========================\n"); 
	return 0;
}

int main() {
	song songs[3];
	song song1, song2, song3;

	strcpy(song1.title, "Dancing Queen");
	strcpy(song1.artist, "ABBA");
	song1.durMin = 3;
	song1.durSec = 50;
	song1.dance = 0.80;

	strcpy(song2.title, "Casual");
	strcpy(song2.artist, "Chappell Roan");
	song2.durMin = 3;
	song2.durSec = 52;
	song2.dance = 0.40;

	strcpy(song3.title, "Friday I'm In Love");
	strcpy(song3.artist, "The Cure");
	song3.durMin = 3;
	song3.durSec = 35;
	song3.dance = 0.60;
  
	songs[0] = song1;
	songs[1] = song2;
	songs[2] = song3;
	printf("Welcome to Rebecca's Awesome Song List!\n");
	printSongs(songs);

	int songID = 0;
	char attribute[32];
	printf("\nEnter a song id to edit [0, 1, 2]: ");
	scanf(" %d", &songID);
	if (songID != 0 && songID != 1 && songID != 2) {
		printf("Invalid choice!\n");
		return 0;
	}
	printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
	scanf(" %s", attribute);
	if ((strcmp(attribute, "artist")) == 0) {
		char inpArtist[32];
		printf("Enter an artist: ");
		scanf(" %[^\n]%*c", inpArtist);
		strcpy(songs[songID].artist, inpArtist); 
	}
	else if ((strcmp(attribute, "title")) == 0) {
        	char inpTitle[32];
        	printf("Enter a title: ");
        	scanf(" %[^\n]%*c", inpTitle);
        	strcpy(songs[songID].title, inpTitle); 
	}
	else if ((strcmp(attribute, "duration")) == 0) {
        	int inpMin;
		int inpSec;
        	printf("Enter a duration (minutes): ");
        	scanf(" %d%*c", &inpMin);
		printf("Enter a duration (seconds): ");
        	scanf(" %d%*c", &inpSec);
        	songs[songID].durMin = inpMin;
		songs[songID].durSec = inpSec; 
	} 
  	else if ((strcmp(attribute, "danceability")) == 0) {
        	float inpDance;
        	printf("Enter danceability: ");
        	scanf(" %f%*c", &inpDance); 
        	songs[songID].dance = inpDance; 
	} 
	else 
		printf("Invalid input!\n");
  
  	printSongs(songs);
	return 0;
}
