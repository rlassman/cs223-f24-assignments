/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 12/1/24
 * Description: Implements a simplified version of the bash command grep using multiple threads.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
#include <bits/pthreadtypes.h>
#include <string.h>

pthread_mutex_t mutex;

struct thread_data {
    char **argv;
    int start;
    int numFiles;
    pthread_t tid;
    int indx;
    int count; //number of lines w/keyword
    char *keyword;
};

void *grep(void *userdata) {
  struct thread_data *data = (struct thread_data *) userdata;
  data->tid = pthread_self();
  int end = data->start + data->numFiles;
  printf("Thread %d searching %d files (%d to %d)\n", data->indx, data->numFiles, data->start, end);

  int count = 0;
  for(int i=data->start; i<end; i++) {
    FILE *infile ;
	  infile = fopen(data->argv[i], "r");
	  if (infile == NULL) {
		  printf("Error, unable to open file %s\n", data->argv[i]);
		  return NULL;
	  }
    char line[100];
    while(fgets(line, 100, infile) != NULL) {
      if(strstr(line, data->keyword)) {
        count++;
        pthread_mutex_lock(&mutex);
        printf("%d) %s : %s", data->indx, data->argv[i], line); 
        pthread_mutex_unlock(&mutex);
      }
    }
    fclose(infile);
  }
  data->count = count;
  return (void*) NULL;
}

int main(int argc, char **argv) {
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  if (argc < 4) {
    printf("Error, not enough arguments");
    exit(0);
  }
  int numThreads = atoi(argv[1]);
  char *keyword = argv[2];
  int totalFiles = argc - 3;
  printf("Searching %d files for keyword: %s\n", totalFiles, keyword);
  
  char **files = (char **)malloc(totalFiles * sizeof(char *));
  for (int i = 0; i < totalFiles; i++) {
        files[i] = (char *)malloc(100 * sizeof(char));
  }
  for (int i = 0; i <totalFiles; i++) {
    strncpy(files[i], argv[i+3], 100);
  }

  int filesPerT;
  int extra;
  if (totalFiles % numThreads != 0) {
    extra = totalFiles % numThreads;
  }
  filesPerT = totalFiles / numThreads;

  pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
  struct thread_data *data = malloc(numThreads * sizeof(struct thread_data));
  pthread_mutex_init(&mutex, NULL);

  int count = 0;
  int start = 0;

  for (int i = 0; i < numThreads; i++) {
    int numFiles = filesPerT;
    if (count < extra) {
      numFiles++;
      count++;
    }
    data[i].indx = i;
    data[i].numFiles = numFiles;
    data[i].keyword = keyword;
    data[i].argv = files;
    data[i].start = start;
    pthread_create(&threads[i], NULL, grep, (void*) &data[i]);
    start += numFiles;
  }
  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Elapsed time is %lf\n", timer);

  for (int i = 0; i < numThreads; i++) {
    printf("Thread %d found %d lines containing keyword: %s\n", data[i].indx, data[i].count, keyword);
  }

  //clean up
  pthread_mutex_destroy(&mutex);
  for(int i=0; i<totalFiles; i++) {
		free(files[i]);
	}
  free(files);
  free(threads);
  free(data);

  return 0;
}
