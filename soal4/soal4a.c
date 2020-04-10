#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

int m1[4][2] =
{
  {1, 2},
  {2, 2},
  {0, 1},
  {1, 1}
};

int m2[2][5] =
{
  {2, 3, 4, 1, 1},
  {1, 2, 2, 0, 3}
};

int step = 0;
int (*hasil)[5][6];

void* multip(void* arg){
  int i, j;

  for(i=0; i<5; i++){
    for(j=0; j<2; j++){
      *hasil[step][i] += m1[step][j] * m2[j][i];
    }
  }
  step++;
}

int main(int argc, char const *argv[]) {
  int i, j, k;

  key_t key = 1234;

  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  hasil = shmat(shmid, NULL, 0);

  printf("Enter 4 x 2 matrix 1\n");

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 2; j++){
      printf("%d  ", m1[i][j]);
    }
    printf("\n");
  }

  printf("\nEnter 2 x 5 matrix 2\n");

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 5; j++) {
      printf("%d  ", m2[i][j]);
    }
    printf("\n");
  }

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      *hasil[i][j] = 0;
    }
  }

  pthread_t threads[4];

  for (i=0; i<4; i++) {
    int* p;
    pthread_create(&threads[i], NULL, multip, (void*)(p));
  }

  for(i=0; i<4; i++){
    pthread_join(threads[i], NULL);
  }

  printf("\nMatrix result\n");

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      printf("%d\t", *hasil[i][j]);
    }
    printf("\n");
  }

  sleep(15);

  shmdt(hasil);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
