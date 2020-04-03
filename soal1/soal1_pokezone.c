#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include <wait.h>

#define MAX 200

int (*item)[4], (*cur_pokemon)[2], *isRunning;

// 1. shutdown game
//    - mematikan program pokezone dan traizone (fork-exec)
void shutdown(){
  pid_t child;
  int status;

  child = fork();

  if(child == 0){
    char *argv[] = {"killall", "1a", NULL};
    execv("/usr/bin/killall", argv);
  }

  else{
    while((wait(&status)) > 0);
    char *argv[] = {"killall", "1b", NULL};
    execv("/usr/bin/killall", argv);
  }
}

// 2. jual item (thread)
//    - stok awal 100
//    - item bertambah 10/10s
//    - maks 200
//    - implementasi lullaby masing-masing 1 threads
void *jualItem(void * ptr){
  int i;

  sleep(10);

  while(*item[0] <= MAX || *item[1] <= MAX || *item[3] <= MAX){
    for(i = 1; i < 4; i++){
      if(*item[i] <= MAX){
        *item[i] += 10;
        printf("ditambah %d [%d]\n", i, *item[i]);
        if(*item[i] > MAX){
          printf("dimaks %d\n", i);
          *item[i] = MAX;
        }
      }
    }
    sleep(10);
  }

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

    if(random > normal){
      if(random > rare){
        printf("legendary Pokemon\n");
        fflush(stdout);
        *cur_pokemon[0] = 3;
      }
      else{
        printf("rare Pokemon\n");
        fflush(stdout);
        *cur_pokemon[0] = 2;
      }
    }
    else{
      printf("normal Pokemon\n");
      fflush(stdout);
      *cur_pokemon[0] = 1;
    }

    if((rand() < prob(0.000125))){
      printf("shiny pokemon\n");
      fflush(stdout);
      *cur_pokemon[1] = 1;
    }
    else{
      *cur_pokemon[1] = 0;
    }
    sleep(1);
  }
}

int main(int argc, char const *argv[]) {
  int i, running = 1;

  key_t key = 1234;
  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  item = shmat(shmid, NULL, 0);
  cur_pokemon = shmat(shmid, NULL, 0);
  isRunning = shmat(shmid, NULL, 0);

  isRunning = &running;

  for(i = 0; i<4; i++){
    *item[i] = 100;
  }

  pthread_t thread[3];

  if(pthread_create(&thread[0], NULL, jualItem, NULL)){
    printf("thread 1 failed\n");
  }

  if(pthread_create(&thread[1], NULL, randomPokemon, NULL)){
    printf("thread 2 failed\n");
  }

  while (running) {
    int input;

    printf("%d %d %d\n", *item[0], *item[1], *item[2]);

    scanf("%d", &input);

    if(input == 1){
      running = 0;
    }
  }

  shmdt(item);
  shmdt(cur_pokemon);
  shmdt(isRunning);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
