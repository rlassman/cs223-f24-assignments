/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 9/27/24
 * Description: Stores songs in a linked list and removes the most danceable
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

struct node{
	song element;
	struct node* next;
};

struct node* head = NULL;

void clearMem() {
        struct node* temp;
	while(head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

struct node* insertFront(song* el) {
	struct node* n = malloc(sizeof(struct node));
	if (n == NULL) {
		printf("Error: Out of space!\n");
		clearMem();
		exit(1);
	}
	n->element = *el;
	n->next = head;
	head = n;
	return n;
}

void printSongs(struct node* list) {
	int i = 0;
	for (struct node* n = list; n != NULL; n = n->next) {
		printf("\n%d) %-30s artist: %-20s duration: %d:%02d  danceability: %.2f energy:  %.2f tempo:  %.2f valence:  %.2f", 
                       i, n->element.title, n->element.artist, n->element.durMin, n->element.durSec, n->element.dance, n->element.energy, 
		       n->element.tempo, n->element.valence);
		i++;
	}
	printf("\n\nData set contains %d songs\n", i);
}

struct node* findDanceable(struct node* list) {
	struct node* d = list;
	for(struct node* n = list->next; n != NULL; n = n->next) {
		if(d->element.dance < n->element.dance) {
			d = n;
		}
	}
	return d;
}

void removeSong(struct node* rem) {
	struct node* prev = NULL;
	for(struct node* n = head; n != rem; n = n->next) {
		prev = n;
	}
	if (rem == head)
		head = rem->next;
	else
		prev->next = rem->next;
	free(rem);
}

int main() {
	FILE *infile ;
	infile = fopen("songlist.csv", "r");
	if (infile == NULL) {
		printf("Error, unable to open file");
		clearMem();
		exit(1);
	}
	char buffer[100];
	fgets(buffer, 100, infile); 

	while(fgets(buffer, 100, infile)) { 
		song s;
		char* tok = strtok(buffer, ",");
		strcpy(s.title, tok);
		tok = strtok(NULL, ",");
		strcpy(s.artist, tok);
		tok = strtok(NULL, ",");
		int dur = atoi(tok)/1000;
		s.durMin = dur/60;
		s.durSec = dur % 60;
		tok = strtok(NULL, ",");
		s.dance = atof(tok);
		tok = strtok(NULL, ",");
        	s.energy = atof(tok);
		tok = strtok(NULL, ",");
        	s.tempo = atof(tok);
		tok = strtok(NULL, ",");
        	s.valence = atof(tok);
		insertFront(&s);
	}
	fclose(infile);
	printSongs(head);

	char inp;
	while (0 == 0) {
		printf("=========================\nPress 'd' to show the most danceable song (any other key to quit): ");
                scanf(" %c", &inp);
		if (inp == 'd') {
			if (head != NULL) {
				struct node* dance = findDanceable(head);
				printf("\nMost Danceable:\n%-20s %-20s (%d:%02d) danceability: %.2f energy:  %.2f tempo:  %.2f valence:  %.2f\n", 
		       	       		dance->element.title, dance->element.artist, dance->element.durMin, 
		               		dance->element.durSec, dance->element.dance, dance->element.energy, 
		               		dance->element.tempo, dance->element.valence);
				printf("------------------------------------------------------------------------------------------------------------------\n");
				removeSong(dance);
			} 
			printSongs(head);
		} else 
			break;
	}
	clearMem();
  	return 0;
}
