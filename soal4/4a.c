#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int i, j, k;
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

  key_t key = 1234;
  int (*hasil)[5][6];

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
      for (k = 0; k < 2; k++) {
        *hasil[i][j] += m1[i][k] * m2[k][j];
      }
    }
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
