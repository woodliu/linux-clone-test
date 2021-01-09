#define _GNU_SOURCE  
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE	65536
int fd;

static int child_fn() {
        printf("PID: %ld\n", (long)getpid());
        sleep (100);
}

int main(int argc, char *argv[])
{
	//Allocate stack for child task
	char *stack = malloc(STACK_SIZE);

	if (!stack) {
		perror("Failed to allocate memory\n");
		exit(1);
	}

        pid_t child_pid = clone(child_fn, stack + STACK_SIZE, CLONE_FILES|SIGCHLD, NULL);
        printf("clone() = %ld\n", (long)child_pid);

	fd = open("file.txt", O_RDWR);
	if (fd == -1) {
		perror("Failed to open file\n");
		exit(1);
	}
		
        waitpid(child_pid, NULL, 0);
        printf("child terminated!\n");
		close(fd);
        sleep (100);

	return 0;
}
