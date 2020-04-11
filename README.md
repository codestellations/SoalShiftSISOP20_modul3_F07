# SoalShiftSISOP20_modul3_F07
Praktikum mata kuliah Sistem Operasi Informatika ITS 2020 Kelompok F07

- Erlinda Argyanti Nugraha  05111840000017
- Lii'zza Aisyah Putri Sulistio   05111840000073

---

# Soal 1
## Traizone
### 1. Normal Mode
#### a. Cari pokemon
menu ini digunakan untuk mencari pokemon dengan ketentuan sebagai berikut :
##### Jika diaktifkan maka setiap 10 detik akan memiliki 60% chance untuk menemui pokemon bertipe normal, rare, legendary dengan encounter rate sesuai pada tabel.
menggunakan thread `*cariPokemon()` sebagai berikut.
~~~
void *cariPokemon(void *ptr){
  thread_0 = pthread_self();
  while(aktif){
    sleep(10);
    if(prob(0.6)){
      printf("\n\nPokemon found!\n\nchoice : ");
      fflush(stdout);
      dapat = true;
      aktif = 0;
    }
    else{
      printf("\n\nNo pokemon found.\n\nchoice : ");
      fflush(stdout);
    }
  }
}
~~~
digunakan thread untuk mencari pokemon dengan keterangan sebagai berikut :
- `prob(0.6)` untuk menghitung kemungkinan 60%
- `dapat` sebagai variabel sudah mendapat pokemon supaya dapat pindah ke **capture mode**.

##### Opsi “Cari Pokemon” akan berubah jadi “Berhenti Mencari” ketika state mencari pokemon aktif.
opsi ini dapat dilihat di fungsi `normal()`.
~~~
  char choice1[20] = "cari pokemon", choice2[20] = "berhenti mencari", choice[20];
  
  .
  .
  .

  if(!aktif){
    strcpy(choice, choice1);
  }
  else{
    strcpy(choice, choice2);
  }
~~~
menu akan berubah dari `cari pokemon` menjadi `berhenti mencari` apabila statusnya *aktif*.

selain itu, digunakan juga fungsi bantuan untuk memasukkan data pokemon yang akan ditangkap.
~~~
void jenisPokemon(){
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

  int r = randNum(5);
  if (*cur_pokemon[0] == 3){
    strcpy(tempPokemon.name, name_legend[r]);
    tempPokemon.encounter = 0.05;
    tempPokemon.escape = 0.2;
    tempPokemon.capture = 0.3;
    tempPokemon.gain_dollar = 200;
  }
  else if(*cur_pokemon[0] == 2){
    strcpy(tempPokemon.name, name_rare[r]);
    tempPokemon.encounter = 0.15;
    tempPokemon.escape = 0.1;
    tempPokemon.capture = 0.5;
    tempPokemon.gain_dollar = 100;
  }
  else{
    strcpy(tempPokemon.name, name_normal[r]);
    tempPokemon.encounter = 0.8;
    tempPokemon.escape = 0.05;
    tempPokemon.capture = 0.7;
    tempPokemon.gain_dollar = 80;
  }
  tempPokemon.available = false;
  tempPokemon.AP = 100;

  if(*cur_pokemon[1] == 1){
    strcat("shining", tempPokemon.name);
    tempPokemon.escape += 0.05;
    tempPokemon.capture -= 0.2;
    tempPokemon.gain_dollar += 5000;
  }
}
~~~

##### State mencari pokemon hanya bisa dihentikan ketika pokemon sudah ditemukan atau trainer memilih untuk berhenti pada menu.
pilihan ini akan dapat diakses setelah memilih untuk `cari pokemon`
~~~
      if(aktif){
        pthread_cancel(thread_0);
        aktif = 0;
      }
      else{
        aktif = 1;
      }
~~~
apabila keadaan `cari pokemon` sudah aktif, maka opsi `berhenti mencari` akan mengcancel thread pencarian pokemon.

##### Jika menemui pokemon maka akan masuk ke Capture Mode.
pilihan ini terdapat dalam menu normal.
~~~
    else if(strcmp("go to capture", input_normal)==0 || strcmp("4", input_normal)==0){
      if(dapat){
        aktif = 0;
        printf("\nmemindahkan ke CAPTURE ...\n");
        sleep(1);
        isCapturing = true;
        capture();
      }
      else{
        printf("pokemon belum ditemukan\n");
      }
    
~~~
masuk ke `capture mode` hanya dapat dilakukan apabila pokemon telah ditemukan, yang ditunjukkan dari variabel `dapat`.

#### b. Pokedex
menu ini digunakan untuk masuk ke pokedex dengan ketentuan :
##### Melihat list pokemon beserta Affection Point (AP) yang dimiliki.
list pokemon dapat dilihat pada menu pokedex.
~~~
    if(strcmp("lihat pokemon", input_poke)==0 || strcmp("1", input_poke)==0){
      for(i = 0; i < 7; i++){
        if(pokemon[i].available == false){
          printf("%d. nama  : %s\n", i+1, pokemon[i].name);
          printf("   AP   : %d\n", pokemon[i].AP);
          ada = 1;
        }

        if(!ada){
          printf("tidak ada pokemon.\n");
        }
      }
    }
~~~
berikut keterangan list pokemon :
- `pokemon[i].available` menunjukkan ketersediaan slot pokemon. **true** berarti slot kosong, **false** berarti slot terisi. dalam hal ini, yang ditampilkan adalah pokemon yang dimiliki, yang berarti menempati slot pokemon.
- `pokemon[i].nama` nama pokemon yang bersangkutan.
- `pokemon[i]].AP` tingkat AP yang dimiliki pokemon yang bersangkutan.

##### Jika menangkap lebih dari 7 maka pokemon yang baru saja ditangkap akan langsung dilepas dan mendapatkan pokedollar sesuai dengan tabel.
implementasinya ada di fungsi `capture()` sebagai berikut.
~~~
          for(i = 0; i < 7 ; i++){
             if(pokemon[i].available){
              .
              .
              .
            }
          }

          if(i == 7){
            printf("slot pokemon penuh.\n");
            printf("pokemon dilepas dan mendapat %d pokedollar.\n", tempPokemon.gain_dollar);
          }
~~~
apabila selama iterasi tidak ada slot yang `available`, maka pokemon dilepas dan medapat uang sebanyak `tempPokemon.gain_dollar`.

##### Setiap pokemon yang dimiliki, mempunyai Affection Point (AP) dengan initial value 100 dan akan terus berkurang sebanyak -10 AP/10 detik dimulai dari waktu ditangkap. AP tidak berkurang jika dalam Capture Mode.
implementasinya ada di thread `*decreaseAP()` sebagai berikut.
~~~
void *decreaseAP(void *pokeAP){
  while(((struct Pokemon*)pokeAP)->available == false){
    while(((struct Pokemon*)pokeAP)->AP <= 100 && isCapturing==false){
      sleep(10);
      ((struct Pokemon*)pokeAP)->AP -= 10;

      if(((struct Pokemon*)pokeAP)->AP == 0){
        if (prob(0.9)){
          printf("pokemon %s melepaskan diri karena kurang perhatian.\n", 
                  ((struct Pokemon*)pokeAP)->name);
          ((struct Pokemon*)pokeAP)->available = true;
          break;
        }
        else{
          printf("pokemon %s reset AP menjadi 50.\n", ((struct Pokemon*)pokeAP)->name);
          ((struct Pokemon*)pokeAP)->AP = 50;
        }
      }
    }
  }
}
~~~
dengan keterangan sebagai berikut :
- `while(((struct Pokemon*)pokeAP)->available == false)` selama pokemon masih ada dalam slot pokedex.
- `while(((struct Pokemon*)pokeAP)->AP <= 100 && isCapturing==false)` selama AP kurang dari 100 dan tidak dalam **capture mode**.
- `((struct Pokemon*)pokeAP)->AP -= 10` pengurangan 10 AP tiap 10 detik (setelah `sleep(10)`).

##### Jika AP bernilai 0, maka pokemon tersebut memiliki 90% chance untuk lepas tanpa memberikan pokedollar ke trainer atau 10% chance untuk reset AP menjadi 50 AP.
apabila AP mencapai 0, maka:
- `prob(0.9)` menghitung kemungkinan 90%, yang mana kemudian pokemon akan melepaskan diri. maka slot pokedex menjadi available `((struct Pokemon*)pokeAP)->available = true`.
- 10% kemungkinan untuk mereset AP menjadi 50, yaitu pada `((struct Pokemon*)pokeAP)->AP = 50`.

##### Di Pokedex trainer juga bisa melepas pokemon yang ditangkap dan mendapat pokedollar sesuai dengan tabel.
pilihan ini terdapat di menu pokedex.
~~~
    else if(strcmp("lepas pokemon", input_poke)==0 || strcmp("2", input_poke)==0){
      for(i = 0; i < 7 ; i++){
        if(!pokemon[i].available){
          printf("slot %d : %s\n", i+1, pokemon[i].name);
          ada = 1;
        }
      }

      if(ada){
        printf("pokemon mana yang akan dilepas : ");
        scanf("%d", &lepas);
        getchar();
        if(!pokemon[lepas-1].available && lepas>0){
          pokemon[lepas-1].available = true;
          pokedollar += pokemon[lepas-1].gain_dollar;
          printf("pokemon %s berhasil dilepas dan mendapat %d dollar.\n", 
                  pokemon[lepas-1].name, pokemon[lepas-1].gain_dollar);
        }
        else{
          printf("pilihan invalid.\n");
        }
      }
      else{
        printf("tidak ada pokemon yang bisa dilepas\n");
      }

    }
~~~
dengan keterangan sebagai berikut :
- `if(!pokemon[i].available)` mengecek slot yang terisi pokemon (available == false).
- `if(!pokemon[lepas-1].available && lepas>0)` mengecek apabila slot pokemon yang akan dilepas benar ada pokemonnya, dan nomor slot yang dimasukkan benar (bukan slot 0 atau negatif).
- `pokemon[lepas-1].available = true` slot pokemon dikosongkan.
- `pokedollar += pokemon[lepas-1].gain_dollar` mendapat pokedollar dari sebanyak `pokemon[].gain_dollar`.

##### Bisa memberi berry ke semua pokemon yang dimiliki untuk meningkatkan AP sebesar +10 (1 berry untuk semua pokemon yang ada di pokedex).
pilihan ini terdapat di menu pokedex.
~~~
    else if(strcmp("beri berry", input_poke)==0 || strcmp("3", input_poke)==0){
      if(berry>0){
        for(i = 0; i < 7; i++){
          if(!pokemon[i].available){
            pokemon[i].AP += 10;
            ada = 1;
          }
        }
        if(ada){
          berry--;
          printf("berry telah diberikan.\n");
        }
        else{
          printf("tidak ada pokemon untuk diberi berry.\n");
        }
      }
      else{
        printf("berry tidak mencukupi.\n");
      }
~~~
dengan keterangan :
- `if(berry>0)` mengecek apabila persediaan berry mencukupi.
- `if(!pokemon[i].available)` mengecek apakah slot benar terisi pokemon.
- `pokemon[i].AP += 10` menambah AP pokemon yang ada sebanyak +10.
- `berry--` mengurangi persediaan berry.

#### c. Shop
menu ini digunakan untuk masuk ke menu shop dengan ketentuan :
##### membeli item dari soal1_pokezone
item yang tersedia adalah `lullaby powder`, `pokeball`, dan `berry`. stok akan dibahas nanti pada bagian **pokezone**.

##### Maksimal masing-masing item yang dapat dibeli dan dipunya oleh trainer adalah 99.
berikut adalah contih pembelian `lullaby powder`. untuk item lain, pembelian prinsipnya sama, yang membedakan adalah harga itemnya.
~~~
  if(jum <= 99 || inv - jum >= 0){
    if(pokedollar > jum*60){
        pokedollar -= jum*60;
        lul += jum;
        inv -= jum;
        *item[1] -= jum;
        printf("lullaby powder dibeli.\n");
    }
~~~
berikut keterangannya 
- `jum <= 99` jumlah yang dibeli maksimal 99.
- `inv - jum >= 0` inventory tidak boleh melebihi kapasitas (maksimal inventory 99).
- `*item[1] -= jum` **item[]** merupakan shared memory yang dipakai bersama dengan `pokezone`, yang merepresentasikan stok item shop.

### 2. Capture Mode
#### a. Tangkap
##### Menangkap menggunakan pokeball. Berhasil ditangkap maupun tidak, pokeball di inventory -1 setiap digunakan.
~~~
    if(strcmp("tangkap", input_capture)==0 || strcmp("1", input_capture)==0){
      if(pokeball>0){
        pokeball--;
~~~
mengurangi `pokeball` apabila mencoba menangkap.
~~~
        if(prob(tempPokemon.capture)){
          printf("pokemon %s berhasil ditangkap!\n", tempPokemon.name);

          sleep(1);

          for(i = 0; i < 7 ; i++){
            if(pokemon[i].available){
              pokemon[i] = tempPokemon;
              printf("%s berhasil disimpan di pokedex.\n", tempPokemon.name);

              if(pthread_create(&thread[i+4], NULL, decreaseAP, pokeAP[i])){
                printf("thread %d failed\n", i+4);
              }

              break;
            }
          }

          if(i == 7){
            printf("slot pokemon penuh.\n");
            printf("pokemon dilepas dan mendapat %d pokedollar.\n", tempPokemon.gain_dollar);
          }
        }
~~~
apabila pokemon tertangkap (kemungkinan berdasarkan `tempPokemon.capture`), maka akan mengecek apabila ada slot kosing atau tidak. kalau ada, pokemon akan disimpan (slot jadi tidak available) dan akan memanggil thread `decreaseAP` untuk pokemon tersebut.

#### Pokemon tersebut memiliki peluang untuk lari dari trainer sesuai dengan persentase escape rate pada tabel.
upaya pokemon untuk lari dijalankan dengan thread `*escapePokemon()` sebagai berikut.
~~~
void *escapePokemon(void *ptr){
  thread_2 = pthread_self();

  while(isCapturing){
    sleep(20);
    if(prob(tempPokemon.escape)){
      printf("pokemon melarikan diri!\n");
      printf("mengembalikan ke MENU ...\n");
      isCapturing = false;
      sleep(1);
      break;
    }
  }
}
~~~
pokemon akan mencoba melarikan diri tiap 20 detik dengan kemungkin sukses lari sebesar `tempPokemon.escape` atau escape rate mereka. apabila berhasil, akan kembali ke menu.

#### b. Item
##### Menggunakan item sesuai dengan tabel item dibawah (hanya lullaby powder).
penggunaan item pada pokemon menggunakan thread `*lullabyPowder` sebagai berikut.
~~~
void *lullabyPowder(void *ptr){
  double tempEscape = tempPokemon.escape;
  tempPokemon.escape = 0.0;
  tempPokemon.capture += 0.2;

  sleep(10);

  tempPokemon.escape = tempEscape;
  tempPokemon.capture -= 0.2;
}
~~~
berikut keterangannya.
- `tempPokemon.escape = 0.0` mengubah escape rate menjadi 0.
- `tempPokemon.capture += 0.2` menambah kemungkinan berhasil ditangkap sebanyak 20%.
- setelah 10 detik, efeknya hilang dan pokemon kembali ke kondisi awal.

#### c. Keluar
##### Keluar dari Capture Mode menuju Normal Mode.
~~~
    else if(strcmp("keluar", input_capture)==0 || strcmp("3", input_capture)==0){
      pthread_cancel(thread_2);
      printf("\nmengembalikan ke MENU ...\n");
      isCapturing = false;
      sleep(1);
      break;
    }
~~~

## Pokezone
### 1. Shutdown game
##### Mematikan program pokezone dan traizone (Gunakan fork-exec).
pada hal ini, program `traizone` diexecute sebagai **1a**, dan program `pokezone` diexecute sebagai **1b**.
~~~
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

  }
}
~~~
- pada child, program **traizone** dimatikan.
- namun program **pokezone** tidak dimatikan di situ, tapi melalui fungsi `main()` setelah memanggil fungsi `shutdown()`.
~~~
  while (running) {
    int input;

    printf("ketik 1 untuk shutdown kedua program\n");

    scanf("%d", &input);

    if(input == 1){
      shutdown();
      running = 0;
    }
~~~
progrma **pokezone** diberhentikan dengan mengubah variabel `running` sehingga loop berhenti dan program berhenti.

### 2. Jual item
#### Stock awal semua item adalah 100.
inisiasi stok berada di fungsi `main()` sebagai berikut.
~~~
  for(i = 0; i<4; i++){
    *item[i] = 100;
  }
~~~
berikut keterangan itemnya.
- `*item[1]` yaitu lullaby powder.
- `*item[2]` yaitu pokeball.
- `*item[3]` yaitu berry.

#### Masing-masing item akan bertambah +10 item/10 detik. Maximum item yang dalam shop adalah 200.
~~~
void *jualItem(void * ptr){
  int i;

  sleep(10);

  while(*item[1] <= MAX || *item[2] <= MAX || *item[3] <= MAX){
    for(i = 1; i < 4; i++){
      if(*item[i] <= MAX){
        *item[i] += 10;
        if(*item[i] > MAX){
          *item[i] = MAX;
        }
      }
    }
    sleep(10);
  }
}
~~~
untuk mengupdate stok shop, digunakan thread yang berjalan tiap 10 detik (dipanggil di `main()`) dan mengupdate shared memory sebagai berikut.
- `*item[1]` yaitu lullaby powder.
- `*item[2]` yaitu pokeball.
- `*item[3]` yaitu berry.
- `if(*item[i] > MAX)` apabila jumlahnya sudah melebihi dari maksimal (200), maka jumlah akan tetap 200.

### 3. Random pokemon
#### Setiap tipe pokemon memiliki peluang 1/8000 untuk jadi shiny pokemon. (Random dulu antara normal, rare, legendary sesuai rate mereka, baru setelah itu di random dengan peluang 1/8000 untuk tahu dia shiny atau tidak).
digunakan thread `*randomPokemon()` untuk mengenerate pokemon.
~~~
void *randomPokemon(void *ptr){
  while(1){
    double random = rand();
    double normal = prob(0.8);
    double rare = normal + prob(0.15);

    if(random > normal){
      if(random > rare){
        *cur_pokemon[0] = 3;
      }
      else{
        *cur_pokemon[0] = 2;
      }
    }
    else{
      *cur_pokemon[0] = 1;
    }

    if((rand() < prob(0.000125))){
      *cur_pokemon[1] = 1;
    }
    else{
      *cur_pokemon[1] = 0;
    }
    sleep(1);
  }
}
~~~
berikut keterangannya.
- `double normal = prob(0.8)` kemungkinan pokemon **normal** adalah 80%.
- `double rare = normal + prob(0.15)` kemungkinan pokemon **rare** adalah 15%.
- kemungkinan pokemon **legendary** adalah sisanya, yaitu 5%.
- setelah dicek kemungkinan masing-masing, dicek kembali kemungkinan **shining** pada `if((rand() < prob(0.000125)))`

untuk mempassing pokemon jenis apa yang digenerate, digunakan shared memory sebagai berikut.
- `*cur_pokemon[1][]` untuk pokemon jenis **normal**.
- `*cur_pokemon[2][]` untuk pokemon jenis **rare**.
- `*cur_pokemon[3][]` untuk pokemon jenis **legendary**.
- `*cur_pokemon[][1]` untuk pokemon jenis **shining**.

thread melakukan kalkulasi ini setiap 1 detik, dengan implementasi pada `sleep(1)`.

---
## Soal 3
Buatlah sebuah program dari C untuk mengkategorikan file. 
Program ini akan memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah sama) ke dalam folder 
sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

### Mengecek apakah path yang dituju merupakan suatu direktori atau bukan
~~~
int is_directory(const char* path)
{
    struct stat path_stat; 
    stat(path, &path_stat); 
    return S_ISDIR(path_stat.st_mode);
}
~~~
- `struct stat path_stat;` struct stat digunakan untuk menyimpan informasi tentang files
- `stat(path, &path_stat); ` fungsi stat digunakan untuk membuat list properti yang diidentifikasi oleh path
- `return S_ISDIR(path_stat.st_mode);` akan memberikan return non-zero jika merupakan sebuah direktori

### Mengambil string nama dari direktori dengan delimiter "/" dan "."
~~~
strcpy(copy, arg);
slash = strtok(copy, "/");
while(slash != NULL)
{
  arrs[x] = slash;
  x++;
  slash = strtok(NULL, "/");
}
strcpy(temp, arrs[x - 1]);
dot = strtok(temp, ".");
while(dot != NULL)
{
   arrd[y] = dot;
   y++;
    dot = strtok(NULL, ".");
}
~~~
- `strcpy(copy, arg);` mengopi arg ke string copy
- `slash = strtok(copy, "/");` digunakan untuk mengambil string dengan delimiter "/"  dan 
  `strcpy(copy, arg);` digunakan untuk mengambil string dengan delimiter "."

Kemudian file dipindahkan ke direktori yang baru dengan nama folder berupa ekstensi dari filenya sesuai dengan perintah input yang dimasukkan dengan penamaan file sama seperti nama dan ekstensi file awal.

### Perintah "-f"
Pada perintah "-f" file yang dipindahkan berjumlah sebanyak argumen yang diinputkan.
<img width="990" alt="Screen Shot 2020-04-11 at 19 17 10" src="https://user-images.githubusercontent.com/58472359/79043909-70237a80-7c2c-11ea-99bb-87aaf1b79ab1.png">
<img width="891" alt="Screen Shot 2020-04-11 at 19 45 18" src="https://user-images.githubusercontent.com/58472359/79044046-2dae6d80-7c2d-11ea-9242-665e3f725835.png">
<img width="891" alt="Screen Shot 2020-04-11 at 19 46 13" src="https://user-images.githubusercontent.com/58472359/79044083-5df60c00-7c2d-11ea-81aa-30d87043fe2e.png">
<img width="891" alt="Screen Shot 2020-04-11 at 19 46 36" src="https://user-images.githubusercontent.com/58472359/79044088-63ebed00-7c2d-11ea-852c-d55bb573f2a5.png">

### Perintah "-d"
Pada perintah "-d" file yang dipindahkan merupakan keseluruhan dalam suatu folder yang dipilih dan direktorinya menjadi 
`folder/direktoribaru`
<img width="990" alt="Screen Shot 2020-04-11 at 19 19 51" src="https://user-images.githubusercontent.com/58472359/79044114-7fef8e80-7c2d-11ea-8305-b7b27623df8b.png">
<img width="990" alt="Screen Shot 2020-04-11 at 19 20 00" src="https://user-images.githubusercontent.com/58472359/79044121-867e0600-7c2d-11ea-8a85-7ca24880b929.png">

### Perintah "*"
Pada perintah "*" file yang dipindahkan merupakan seluruh file yang berada dalam satu folder dengan program soal3.c 
yang sedang dijalankan
<img width="990" alt="Screen Shot 2020-04-11 at 19 20 27" src="https://user-images.githubusercontent.com/58472359/79045609-3b1c2580-7c36-11ea-8ad6-9de0fd200ec6.png">
<img width="891" alt="Screen Shot 2020-04-11 at 19 48 33" src="https://user-images.githubusercontent.com/58472359/79044144-b75e3b00-7c2d-11ea-9120-9f47624fe2f9.png">

---

# Soal 4
## a. Perkalian matriks
### 1. Ukuran matriks pertama adalah 4x2, dan matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
inisialisasi matriks awal adalah sebagai berikut.
~~~
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
~~~
dengan hasilnya nanti akan dimasukkan ke dalam matriks yang merupakan shared memory dengan `4b`.
~~~
int (*hasil)[5][6];
~~~

perkalian matriks dilakukan menggunakan thread `*multip()` sebagai berikut.
~~~
void* multip(void* arg){
  int i, j;

  for(i=0; i<5; i++){
    for(j=0; j<2; j++){
      *hasil[step][i] += m1[step][j] * m2[j][i];
    }
  }
  step++;
}
~~~
di mana thread dibuat per baris matriks. dalam hal ini, karena baris matriks hasil ada 4, maka dibuat 4 thread. berikut adalah pemanggilan dan join thread pada fungsi `main()`.
~~~
  pthread_t threads[4];

  for (i=0; i<4; i++) {
    int* p;
    pthread_create(&threads[i], NULL, multip, (void*)(p));
  }

  for(i=0; i<4; i++){
    pthread_join(threads[i], NULL);
  }
~~~

### 2. Tampilkan matriks hasil perkalian tadi ke layar.
setelah thread dijoin, hasil ditampilkan.
~~~
  printf("\nMatrix result\n");

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      printf("%d\t", *hasil[i][j]);
    }
    printf("\n");
  }
~~~

## b. Deret Aritmatika
### 1. Program ini akan mengambil variabel hasil perkalian matriks dari program "4a.c" (program sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
hasil yang akan ditampilkan ini adalah shared memory dengan `4a`. berikut inisialisasi matriks `hasil`.
~~~
int (*hasil)[5][6];
~~~
dan berikut untuk menampilkannya.
~~~
  printf("\nMatrix result\n");

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      printf("%d\t", *hasil[i][j]);
    }
    printf("\n");
  }
~~~

### 2. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks tersebut, carilah nilai hasil penjumlahan (deret aritmatika), dan tampilkan hasilnya ke layar dengan format seperti matriks.
penjumlahan dari 1 + 2 + 3 + ... + n adalah deret aritmatika dengan beda = 1 dan Un = n. maka dapat kita gunakan rumus Sn aritmatika. untuk menghitung penjumlahan ini, digunakan thread yang menghitung tiap elemen matriks `hasil`. berikut adalah threadnya.
~~~
void *aritmatika(void *ptr){
  int n = *hasil[row][col];
  int sn = (n * (1 + n))/2;

  fact[row][col] = sn;

  if(col == 4){
    row++;
    col = 0;
  }
  else{
    col++;
  }
}
~~~
hasil dari penjumlahan ini akan disimpan di dalam matriks baru yaitu `fact[][]`.

berikut adalah pemanggilan thread dan joinnya. dibuat 20 thread karena banyak elemen dalam matriks `hasil` adalah 4 x 5 = 20.
~~~
  pthread_t threads[20];

  for (i=0; i<20; i++) {
    int* p;
    pthread_create(&threads[i], NULL, aritmatika, (void*)(p));
  }

  for(i=0; i<20; i++){
    pthread_join(threads[i], NULL);
  }
~~~

selanjutnya menampilkan hasil penjumlahan, yaitu matriks `fact`.
~~~
  printf("\nSum result\n");

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++) {
      printf("%d\t", fact[i][j]);
    }
    printf("\n");
  }
~~~

## c. ls | wc -l
### Diminta mengetahui jumlah file dan folder di direktori saat ini dengan command "ls | wc -l".
untuk mengerjakan soal ini, digunakan pipe dan fork. pada child, dilakukan `ls` seperti tertera berikut.
~~~
  if (child == 0){
    dup2(fd1[1], 1);
    close(fd1[0]);
    close(fd1[1]);

    char *argv[2] = {"ls", NULL};
    execv("/bin/ls", argv);
  }
~~~

pada parent, dilakukan `wc -l` seperti tertera berikut.
~~~
  else{
    dup2(fd1[0], 0);
    close(fd1[0]);
    close(fd1[1]);

    char *argv[3] = {"wc", "-l", NULL};
    execv("/usr/bin/wc", argv);
  }
~~~


