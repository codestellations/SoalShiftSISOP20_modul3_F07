//
//  modul3_soal3.c
//  modul3_no3
//
//  Created by Liizza Sulistio on 04/01/20.
//  Copyright © 2020 Liizza Sulistio. All rights reserved.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <limits.h>
#include <dirent.h>

pthread_t tid[1000];
pid_t child;

char cwd[1024] = {0};

int main(int argc, char const *argv[])
{
    int loop = 0;
    if(strcmp(argv[1], "-f") == 0)
    {
        
    }
    else if(strcmp(argv[1], "*") == 0)
    {
        
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        
    }
    else
    {
        exit(0);
    }
    
    
    
}
