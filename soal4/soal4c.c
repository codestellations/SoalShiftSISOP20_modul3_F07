#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	int fd1[2];

	pid_t child, child1;

	if (pipe(fd1)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}

	child = fork();

	if (child  < 0)
	{
		fprintf(stderr, "fork Failed" );
		return 1;
	}

  if (child == 0){
    dup2(fd1[1], 1);
    close(fd1[0]);
    close(fd1[1]);

    char *argv[2] = {"ls", NULL};
    execv("/bin/ls", argv);
  }

  else{
    dup2(fd1[0], 0);
    close(fd1[0]);
    close(fd1[1]);

    char *argv[3] = {"wc", "-l", NULL};
    execv("/usr/bin/wc", argv);
  }
}
