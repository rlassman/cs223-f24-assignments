/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 12/1/24
 * Description: Uses multiple threads to create a BST storing files, then lists their dependencies.
 ---------------------------------------------*/
#include <stdio.h>
#include "tree.h"
#include "stdlib.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

struct thread_data {
    char **argv;
    int start;
    int numFiles;
    pthread_t tid;
    int indx;
    struct tree_node** root;
};

void *buildTree(void *userdata) {
  struct thread_data *data = (struct thread_data *) userdata;
  data->tid = pthread_self();
  int end = data->start + data->numFiles;
  printf("Thread %d processing %d files (%d to %d)\n", data->indx, data->numFiles, data->start, end);

  for(int i=data->start; i<end; i++) {
    if (access(data->argv[i], F_OK) != 0) {
      printf("Error, unable to open file %s\n", data->argv[i]);
		  return NULL;
    }
    pthread_mutex_lock(&mutex);
    *(data->root) = insert(data->argv[i], *(data->root));
    pthread_mutex_unlock(&mutex);
  }

  return (void*) NULL;
}

int main(int argc, char **argv) {
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  if (argc < 3) {
    printf("Error, not enough arguments");
    exit(0);
  }
  int numThreads = atoi(argv[1]);
  int totalFiles = argc - 2;
  printf("Processing %d files\n", totalFiles);
  char **files = (char **)malloc(totalFiles * sizeof(char *));
  for (int i = 0; i < totalFiles; i++) {
        files[i] = (char *)malloc(100 * sizeof(char));
  }
  for (int i = 0; i <totalFiles; i++) {
    strncpy(files[i], argv[i+2], 100);
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

  struct tree_node* root = NULL;

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
    data[i].argv = files;
    data[i].start = start;
    data[i].root = &root;
    pthread_create(&threads[i], NULL, buildTree, (void*) &data[i]);
    start += numFiles;
  }
  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Elapsed time is %lf\n", timer);

  char inp[100];
  while(0 == 0) {
    scanf(" %s", inp);
    if (strcmp(inp, "list") == 0) {
      printSorted(root);
    } else if (strcmp(inp, "quit") == 0) {
      break;
    } else {
      struct tree_node* file = find(inp, root);
      if (file == NULL) {
        printf(" %s not found\n", inp);
      } else {
        printf(" %s has the following dependencies\n", inp);
        FILE *infile ;
	      infile = fopen(file->data.name, "r");
        char line[100];
        while(fgets(line, 100, infile) != NULL) {
          if(strstr(line, "#include")) {
            char dep[100];
            char *start = strchr(line, '"');
            if (start) {
              char *end = strchr(start + 1, '"');
              int len = end - start - 1;
              strncpy(dep, start + 1, len);
              dep[len] = '\0'; 
              printf("     %s\n", dep);
            } else {
              char *start = strchr(line, '<');
              char *end = strchr(start + 1, '>');
              int len = end - start - 1;
              strncpy(dep, start + 1, len);
              dep[len] = '\0'; 
              printf("     %s\n", dep);
            }
          }
        }
        fclose(infile);
      }
    }
  }
  //Clean Up
  clear(root);
  pthread_mutex_destroy(&mutex);
  for(int i=0; i<totalFiles; i++) {
		free(files[i]);
	}
  free(files);
  free(threads);
  free(data);
  return 0;
}
