//
//  modul3_soal3.c
//  modul3_no3
//
//  Created by Liizza Sulistio on 04/01/20.
//  Copyright © 2020 Liizza Sulistio. All rights reserved.
//

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <limits.h>
#include <dirent.h>

pthread_t tid[1000]; /*menginisiasi banyak maksimal thread*/
pid_t child_id;
char cwd[1024] = {0};

int is_directory(const char* path) /*mengecek apakah suatu direktori atau bukan*/
{
    struct stat path_stat; /*struct stat menyimpan informasi tentang files*/
    stat(path, &path_stat); /*untuk membuat list properti yg diidentifikasi stat*/
    return S_ISDIR(path_stat.st_mode); /*return non-zero jika file merupakan direktori*/
}

void *count(void *arg)
{
    if (!is_directory(arg))
    {
        char copy[1024] = {0};
        memset(copy, 0, 1024);

        char *slash, *dot;
        int x = 0, y = 0;
        char *arrs[4], *arrd[10], extension[100], temp[100];
        memset(arrs, 0, 4);
        memset(arrd, 0, 10);
        memset(extension, 0, 100);
        memset(temp, 0, 100);

        strcpy(copy, arg); /*mengopi string ke variabel copy*/
        slash = strtok(copy, "/"); /*split string dengan delimiter / */
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

        strcpy(extension, arrd[y-1]);
        for(int i=0 ; i < strlen(extension);i++)
        {
            extension[i] = tolower(extension[i]);
        }

        char temp1[1024] = {0};
        char directory[1024] = {0};
        memset(temp1, 0, 1024);
        memset(directory, 0, 1024);
        strcpy(temp1, "/home/el/Documents/sisop_modul3/soal3/");
        
        if(y == 2)
        {
            strcpy(directory, arrd[y-1]);
            strcat(temp1, directory);
            mkdir(temp1, 0777);
        }else
        {
            strcpy(directory, "Unknown");
            strcat(temp1, directory);
            mkdir(temp1, 0777);
        }

        int ch;
        FILE *pf1, *pf2;
        pf1 = fopen(arg, "r");
        
        strcat(temp1, "/");
        strcat(strcat(strcat(temp1, arrd[y-2]), "."), extension);

        pf2 = fopen(temp1, "w");
        while ((ch = fgetc(pf1)) != EOF)
        {
            fputc(ch, pf2);
        }
        remove(arg);
        fclose(pf1);
        fclose(pf2);
    }
}

int main(int argc, char const *argv[])
{
    int loop = 0, i = 0;
    if(strcmp(argv[1], "-f") == 0)
    {
        for(i=2; i<argc; i++){
            pthread_create(&(tid[i]), NULL, count, (void *)argv[i]);
            pthread_join(tid[i], NULL);
        }
    }
    else if(strcmp(argv[1], "*") == 0)
    {
        getcwd(cwd, sizeof(cwd));
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (cwd)) != NULL) {
            char temp[1024] = "\0";
            while ((ent = readdir (dir)) != NULL) {
                strcpy(temp, cwd);
                strcat(temp, "/");
                strcat(temp, ent->d_name);
                
                printf ("%s\n", temp);
                
                char a[50];
                char b[50];
                strcpy(a, cwd);
                strcat(a, "..");
                strcpy(b, cwd);
                strcat(b, ".");
                if(strcmp(temp,a)==0 || strcmp(temp,b)==0){
                    continue;
                }
                if(!is_directory(temp)){
                    pthread_create(&(tid[loop]), NULL, count, (void *)temp);
                    loop++;
                }
                for(int j=0; j<loop; j++){
                    pthread_join(tid[j], NULL);
                }
            }
            closedir (dir);
        }
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        getcwd(cwd, sizeof(cwd));
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (argv[2])) != NULL) {
            char temp[1024] = "\0";
            while ((ent = readdir (dir)) != NULL) {
                strcpy(temp, argv[2]);
                strcat(temp, ent->d_name);
                
                printf ("%s\n", temp);
                
                char a[50];
                char b[50];
                strcpy(a, cwd);
                strcat(a, "..");
                strcpy(b, cwd);
                strcat(b, ".");
                if(strcmp(temp,a)==0 || strcmp(temp,b)==0) continue;
                if(!is_directory(temp))
                {
                    pthread_create(&(tid[loop]), NULL, count, (void *)temp);
                    loop++;
                }
                for(int j=0; j<loop; j++){
                    pthread_join(tid[j], NULL);
                }
            }
            closedir (dir);
        }
    }
    else
    {
        exit(0);
    }
}
