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

int main(){
  int key = ftok("makefile",22);
  int semid = semget(key, 1,0);
  struct sembuf s;
  s.sem_num = 0;
  s.sem_op = -1;
  s.sem_flg = SEM_UNDO;
  printf("semval %d\n", semctl(semid,0,GETVAL));
  semop(semid, &s, 1);
  printf("semval %d\n", semctl(semid,0,GETVAL));
  int shmid = shmget(key, sizeof(int), 0644);
  printf("shmid %d\n", shmid);
  int *attached = (int *)(shmat(shmid,0,0));
  int *mem = attached;
  int f = open("story.txt", O_RDWR | O_APPEND, 0644);
  int bytes = *mem;
  char *content;
  int size;
  struct stat *fs = (struct stat *) malloc(sizeof(struct stat));
  f = open("story.txt", O_RDWR, 0644);
  stat("story.txt", fs);
  size = fs->st_size;
  int seeker = lseek(f,((-1)*bytes), SEEK_END);
  printf("the size of the file is %d and we are at %d we tried to be %d\n",size, seeker, (-1) * bytes);
  read(f,content,bytes);
  content[bytes + 1] = '\0';
  printf("%s\n", content);
  printf("enter Whats next? :");
  char newcontent[100];
  fgets(newcontent,sizeof(newcontent), stdin);
  printf("%s\n", newcontent);
  write(f, newcontent, strlen(newcontent));
  *attached = strlen(newcontent);
  mem = attached;
  shmdt(attached);
  s.sem_op = 1;
  semop(semid,&s,1);
  close(f);
  return 0;
}
