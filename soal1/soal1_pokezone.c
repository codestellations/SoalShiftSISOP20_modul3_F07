#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>

// 1. shutdown game
//    - mematikan program pokezone dan traizone (fork-exec)
void shutdown(){
  pid_t child;
  int status;

  child = fork();

  if(child == 0){
    char *argv[3] = {"killall", "pokezone", NULL};
    execv("usr/bin/killall", argv);
  }
  else{
    while((wait(&status)) > 0);
    char *argv[3] = {"killall", "traizone", NULL};
    execv("usr/bin/killall", argv);
  }
}

// 2. jual item (thread)
//    - stok awal 100
//    - item bertambah 10/10s
//    - maks 200
//    - implementasi lullaby masing-masing 1 threads
void *jualItem(void * ptr){

}

// 3. random Pokemon
//    - peluang 1/8000 jadi shiny
//    - peluang masing2 golongan, baru dipeluang shiny
//    - shiny escape rate +5%, dollar +5000
//    - kalkulasi random tiap 1 detik
double prob(double probability){
  return probability * ((double)RAND_MAX + 1.0);
}

void *randomPokemon(void *ptr){
  while(1){
    double random = rand();
    double normal = prob(0.8);
    double rare = normal + prob(0.15);
    // double legend = rare + prob(0.05);

    if(random > normal){
      if(random > rare){
        printf("legendary Pokemon\n");
      }
      else{
        printf("rare Pokemon\n");
      }
    }
    else{
      printf("normal Pokemon\n");
    }

    if((rand < prob(0.000125)){
      printf("shiny pokemon\n");
    }
    else{
      printf("normal pokemon\n");
    }
  }
}

int main(int argc, char const *argv[]) {
  int *hasil;

  key_t key = 1234;
  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  hasil = shmat(shmid, NULL, 0);

  // pthread_t thread[20];
  //
  // for (i=0; i<20; i++) {
  //   int* p;
  //   pthread_create(&threads[i], NULL, factorial, (void*)(p));
  // }
  //
  // for(i=0; i<20; i++){
  //   pthread_join(threads[i], NULL);
  // }

  shmdt(hasil);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
