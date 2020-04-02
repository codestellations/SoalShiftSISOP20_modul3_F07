#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

int row = 0, col = 0;
char fact[5][6];

int (*hasil)[5][6];

void* factorial(void* arg){
  int a[200],n,counter,temp,i, index=0, x;
  char str[500];

  a[0]=1;
  counter=0;

  // printf("[%d] ", *hasil[row][col]);

  for(x = *hasil[row][col]; x >= 2; x--)
  {
      temp=0;
      for(i=0; i<=counter; i++)
      {
          temp=(a[i]*x)+temp;
          a[i]=temp%10;
          temp=temp/10;
      }
      while(temp>0)
      {
          a[++counter]=temp%10;
          temp=temp/10;
      }
  }
  for(i=counter; i>=0; i--){
    // printf("%d",a[i]);
    index += sprintf(&str[index], "%d", a[i]);
  }
  printf("%s\t\t", str);

  if(col == 4){
    row++;
    col = 0;
    printf("\n");
  }
  else{
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
  printf("\nFactorial result\n");

  pthread_t threads[20];

  for (i=0; i<20; i++) {
    int* p;
    pthread_create(&threads[i], NULL, factorial, (void*)(p));
  }

  for(i=0; i<20; i++){
    pthread_join(threads[i], NULL);
  }

  // for (i = 0; i < 4; i++) {
  //   for (j = 0; j < 5; j++) {
  //     // printf("%d\t", *hasil[i][j]);
  //     // fact[i][j] = *hasil[i][j];
  //     printf("%d\t", fact[i][j]);
  //   }
  //   printf("\n");
  // }

  shmdt(hasil);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
