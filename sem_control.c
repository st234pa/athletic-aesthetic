#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  union semun {
    int     val;            /* value for SETVAL */
    struct  semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    u_short *array;         /* array for GETALL & SETALL */
  };
  int semid;
  int key = ftok("makefile", 22);
  int sc;
  int shm;
  int sd;
  int f;
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0) {
    shm = shmget(key, sizeof(int), IPC_CREAT | 0644);
    semid= semget(key, 1, IPC_CREAT | 0644);
    if (semid >=0) {
      int value = 1;
      if (argv[2] != NULL) value = atoi(argv[2]);
      union semun su;
      su.val = value;
      sc = semctl(semid, 0, SETVAL, su);
      printf("semaphore value set: %d\n", sc);
      f = open("story.txt", O_CREAT | O_TRUNC, 0644);
      close(f);
    }
    else {
      printf("semaphore already exists");
    }
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0) {
    char *content;
    long size;
    struct stat *fs = (struct stat *) malloc(sizeof(struct stat));
    f = open("story.txt", O_RDONLY, 0644);
    stat("story.txt", fs);
    size = fs->st_size;
    
    printf("%d\n", size);
    int err = read(f,content,size);
    content[size+1] = 0;
    
    printf("%s/n", content);
    free(fs);
    close(f);
  }
  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    char *content;
    long size;
    struct stat *fs = (struct stat *) malloc(sizeof(struct stat));
    f = open("story.txt", O_RDONLY, 0644);
    stat("story.txt", fs);
    size = fs->st_size;
    
    int err = read(f,content,size+1);
    content[size+1] = 0;
    printf("%s/n", content);
    /*semid=semget(key, 1, 0);
    sc = semctl(semid, 0, IPC_RMID);
    sd = shmget(shm, sizeof(int), 0644);*/
    free(fs);
    close(f);
  }
  return 0;
}
