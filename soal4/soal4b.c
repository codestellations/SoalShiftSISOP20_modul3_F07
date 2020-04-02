#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

int row = 0, col = 0;
char fact[5][6];

int (*hasil)[5][6];

void *aritmatika(void *ptr){
  int n = *hasil[row][col];
  int sn = (n / 2) * (1 + n);

  if(col == 4){
    printf("%d\n", sn);
    row++;
    col = 0;
  }
  else{
    printf("%d\t", sn);
    col++;
  }
}

int main(int argc, char const *argv[]) {
  int i, j, k;

  key_t key = 1234;

  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  hasil = shmat(shmid, NULL, 0);

  printf("\nMatrix result\n");

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      printf("%d\t", *hasil[i][j]);
    }
    printf("\n");
  }
  printf("\nSum result\n");

  pthread_t threads[20];

  for (i=0; i<20; i++) {
    int* p;
    pthread_create(&threads[i], NULL, aritmatika, (void*)(p));
  }

  for(i=0; i<20; i++){
    pthread_join(threads[i], NULL);
  }

  shmdt(hasil);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
