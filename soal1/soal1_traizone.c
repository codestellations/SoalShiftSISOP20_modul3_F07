#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>

int (*item)[4], (*cur_pokemon)[2], *isRunning;

int aktif = 0, pokeball = 9, pokedollar = 100, lul = 0, berry = 0,
    cur = 0;
bool dapat = true, isPokedex = false, isShop = false,
     isCapturing = false, isAPDecreasing = true;

pthread_t thread[20], thread_0;

struct Pokemon{
  bool available = 1;
  char name[20];
  double encounter;
  double escape;
  double capture;
  int gain_dollar;
  int AP;
} pokemon[7];

void normal();
void capture();
void pokedex();
void shop();

bool prob(double probability){
  return rand() < probability * ((double)RAND_MAX + 1.0);
}

int randNum(int num){
  return rand() % num;
}

// 1.a cari pokemon
//     - aktif > tiap 10 detik ada 60% chance menemukan pokemon
//       (encounter berdasarkan pokezone)
//     - cari pokemon AKTIF > berhenti mencari
//     - menemukan pokemon > masuk capture mode
//     - boleh menambah menu go to capture mode

void jenisPokemon(){
  int i;

  char name_normal[5][15] =
  {
    "Bulbasaur", "Charmander", "Squirtle", "Rattata", "Caterpie"
  };

  char name_rare[5][15] =
  {
    "Pikachu", "Eevee", "Jigglypuff", "Snorlax", "Dragonite"
  };

  char name_legend[5][15] =
  {
    "Mew", "Mewtwo", "Moltres", "Zapdos", "Articuno"
  };

  for(i = 0; i<7; i++){
    if(pokemon[i].available){
      int r = randNum(5);
      if (*cur_pokemon[0] == 3){
        strcpy(pokemon[i].name, name_legend[r]);
        pokemon[i].encounter = 0.05;
        pokemon[i].escape = 0.2;
        pokemon[i].capture = 0.3;
        pokemon[i].gain_dollar = 200;
      }
      else if(*cur_pokemon[0] == 2){
        strcpy(pokemon[i].name, name_rare[r]);
        pokemon[i].encounter = 0.15;
        pokemon[i].escape = 0.1;
        pokemon[i].capture = 0.5;
        pokemon[i].gain_dollar = 100;
      }
      else{
        strcpy(pokemon[i].name, name_normal[r]);
        pokemon[i].encounter = 0.8;
        pokemon[i].escape = 0.05;
        pokemon[i].capture = 0.7;
        pokemon[i].gain_dollar = 80;
      }
      pokemon[i].available = false;
    }
  }
  // no slot available
  printf("no slot available. \n");
}

void dapatPokemon(){
  char input_cari;

  dapat = true;

  printf("\nMove to capture scene? y/n\n\nchoice : ");
  scanf("%c", &input_cari);
  getchar();

  if(input_cari == 'y'){
    aktif = 0;
    printf("memindahkan ke CAPTURE ...\n");
    sleep(1);
    isCapturing = true;
    capture(pokeball);
  }
  else if (input_cari == 'n'){
    printf("mengembalikan ke MENU ...\n");
    sleep(1);
  }
  else{
    printf("perintah invalid. mengembalikan ke MENU ...\n");
    sleep(1);
  }

  dapat = false;
}

void *cariPokemon(void *ptr){
  thread_0 = pthread_self();
  while(aktif){
    sleep(5);
    if(prob(0.6)){
      printf("\n\nPokemon found!\n\nchoice : ");
      fflush(stdout);
      dapat = 1;
      aktif = 0;
    }
    else{
      printf("\n\nNo pokemon found.\n\nchoice : ");
      fflush(stdout);
    }
  }
}

// void *decreaseAP(void *ptr){
//   int ap = 100;
//
//   while(pokemon[cur].AP <= 100 && isCapturing==false){
//
//     sleep(10);
//     ap -= 10;
//   }
// }

// 1.b pokedex
//     - list pokemon + affection point
//     - maks 7 pokemon
//     - > 7 maka yang baru ditangkap akan dilepas dan dapat duit
//     - AP initial value 100, -10 tiap 10 detik sejak ditangkap
//     - AP == 0, 90% chance lepas tanpa duit atau 10% reset AP jadi 50
//     - AP tidak berkurang dalam capture mode
//     - bisa melepas dan dapat duit sesuai tabel
//     - bisa beri berry ke semua, +10 AP ke semua, -1 berry
void pokedex(){
  int i;

  printf("\nPOKEDEX\n");

  for(i = 0; i < 7; i++){
    strcpy(pokemon[i].name, "bulbasaur");
    pokemon[i].encounter = 0.8;
    pokemon[i].escape = 0.5;
    pokemon[i].capture = 0.7;
    pokemon[i].gain_dollar = 80;
    pokemon[i].AP = 100;
  }

  for(i = 0; i < 7; i++){
    printf("%d. nama  : %s\n", i+1, pokemon[i].name);
    printf("   AP  : %d\n", pokemon[i].AP);
  }

  isPokedex = false;
}

// 1.c shop
//     - membeli item dari pokezone
//     - maks 99 (beli atau punya)
void shop(){
  int jum;
  char input_shop[20];

  int inv = 99 - (lul + pokeball + berry);

  while(isShop){
    printf("\nSHOP\n1. lullaby powder [%d]\n2. pokeball [%d]\n3. berry [%d]\n4. keluar", *item[1], *item[2], *item[3]);
    printf("\n--------\nyour inventory  : %d [lullaby %d] [pokeball %d] [berry %d]", inv, lul, pokeball, berry);
    printf("\nyour pokedollar : %d\n--------", pokedollar);
    printf("\nchoice : ");

    fgets(input_shop, 20, stdin);
    input_shop[strlen(input_shop)-1] = '\0';

    if(strcmp("lullaby powder", input_shop)==0 || strcmp("1", input_shop)==0){
      printf("ini lullaby\n");
      printf("jumlah : ");
      scanf("%d", &jum);

      if(jum <= 99 || inv - jum >= 0){
        if(pokedollar > jum*60){
            pokedollar -= jum*60;
            lul += jum;
            inv -= jum;
            *item[1] -= jum;
            printf("lullaby powder dibeli.\n");
        }

        else
        printf("pokedollar tidak mencukupi.\n");
      }
      else{
        printf("jumlah melebihi batas\n");
      }
    }

    else if(strcmp("pokeball", input_shop)==0 || strcmp("2", input_shop)==0){
      printf("ini pokeball\n");

      printf("jumlah : ");
      scanf("%d", &jum);

      if(jum <= 99 || inv - jum >= 0){
        if(pokedollar > jum*5){
          pokedollar -= jum*5;
          pokeball += jum;
          inv -= jum;
          *item[2] -= jum;
          printf("pokeball dibeli.\n");
        }

        else
        printf("pokedollar tidak mencukupi.\n");
      }
      else{
        printf("jumlah melebihi batas\n");
      }
    }

    else if(strcmp("berry", input_shop)==0 || strcmp("3", input_shop)==0){
      printf("ini berry\n");

      printf("jumlah : ");
      scanf("%d", &jum);

      if(jum <= 99 || inv - jum >= 0){
        if(pokedollar > jum*15){
          pokedollar -= jum*15;
          berry += jum;
          inv -= jum;
          *item[3] -= jum;
          printf("berry dibeli.\n");
        }

        else
        printf("pokedollar tidak mencukupi.\n");
      }
      else{
        printf("jumlah melebihi batas\n");
      }

    }

    else if(strcmp("keluar", input_shop)==0 || strcmp("4", input_shop)==0){
      printf("mengembalikan ke MENU ...\n");
      isShop = false;
      sleep(1);
      break;
    }

    else{
      printf("pilihan invalid\n");
    }
    getchar();
  }
  return;

}

// 2. capture mode
// 2.a tangkap >> -1 pokeball di inventory (berhasil maupun tidak)
// 2.b item >> menggunakan item LULLABY POWDER
// 2.c keluar >> kembali ke normal mode
// *** pokemon memiliki peluang untuk lari berdasarkan tabel
void capture(){
  char input_capture[20];

  jenisPokemon();

  while(isCapturing){
    printf("\nCATCH the pokemon!\n1. tangkap\n2. item\n3. keluar\nchoice : ");

    fgets(input_capture, 20, stdin);
    input_capture[strlen(input_capture)-1] = '\0';

    if(strcmp("tangkap", input_capture)==0 || strcmp("1", input_capture)==0){
      printf("ini nangkep\n");
      if(pokeball>0){
        pokeball--;
      }
      else
      printf("pokeball habis.\n");
    }

    else if(strcmp("item", input_capture)==0 || strcmp("2", input_capture)==0){
      printf("ini item\n");
    }

    else if(strcmp("keluar", input_capture)==0 || strcmp("3", input_capture)==0){
      printf("mengembalikan ke MENU ...\n");
      isCapturing = false;
      sleep(1);
      break;
    }

    else{
      printf("pilihan invalid\n");
    }
  }
  return;
}

// 1. normal mode
void normal(){
  char choice1[20] = "cari pokemon", choice2[20] = "berhenti mencari", choice[20];
  char input_normal[20];

  strcpy(choice, choice1);

  printf("WELCOME TO POKE*ZONE!!!!\n-------------------------\n");
  while(!(isCapturing && isShop && isPokedex)){
    // if(dapat){
    //   dapatPokemon();
    // }
    // else{
      if(!aktif){
        strcpy(choice, choice1);
      }
      else{
        strcpy(choice, choice2);
      }

      printf("MENU\n1. %s\n2. pokedex\n3. shop\n4. go to capture\n5. exit\nchoice : ", choice);

      fgets(input_normal, 20, stdin);
      input_normal[strlen(input_normal)-1] = '\0';

      if(strcmp(choice, input_normal)==0 || strcmp("1", input_normal)==0){
        printf("ini pilihan 1\n");

        if(pthread_create(&thread[0], NULL, cariPokemon, NULL)){
          printf("thread 1 failed\n");
        }

        if(aktif){
          pthread_cancel(thread_0);
          aktif = 0;
        }
        else{
          aktif = 1;
        }
      }

      else if(strcmp("pokedex", input_normal)==0 || strcmp("2", input_normal)==0){
        // printf("ini pilihan 2\n");
        isPokedex = true;
        pokedex();
      }

      else if(strcmp("shop", input_normal)==0 || strcmp("3", input_normal)==0){
        isShop = true;
        shop();
      }

      else if(strcmp("go to capture", input_normal)==0 || strcmp("4", input_normal)==0){
        if(dapat){
          dapatPokemon();
        }
        else{
          printf("pokemon belum ditemukan\n");
        }
      }

      else if(strcmp("exit", input_normal)==0 || strcmp("5", input_normal)==0)
      break;

      else{
        printf("pilihan invalid\n");
      }

      printf("\n");
    // }

  }
}

int main(int argc, char const *argv[]) {
  key_t key = 1234;
  int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
  item = shmat(shmid, NULL, 0);
  cur_pokemon = shmat(shmid, NULL, 0);
  isRunning = shmat(shmid, NULL, 0);

  normal();

  shmdt(item);
  shmdt(cur_pokemon);
  shmdt(isRunning);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
