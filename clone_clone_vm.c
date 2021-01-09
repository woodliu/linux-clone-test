#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/mman.h>
#include <errno.h>

#include <sys/stat.h>

#define STACK_SIZE      65536
int fd;

static int child_fn() {
        printf("PID: %ld\n", (long)getpid());
        sleep (100);
}

int main(int argc, char *argv[])
{
	int fd = 0;
	char *ptr = NULL;
	struct stat buf = {0};
		
        //Allocate stack for child task
        char *stack = malloc(STACK_SIZE);

        if (!stack) {
                perror("Failed to allocate memory\n");
                exit(1);
        }

        pid_t child_pid = clone(child_fn, stack + STACK_SIZE, CLONE_VM |SIGCHLD, NULL);
        printf("clone() = %ld\n", (long)child_pid);

	if ((fd = open("file.txt", O_RDWR)) < 0)
	{
		printf("open file error\n");
		return -1;
	}

	if (fstat(fd, &buf) < 0)
	{
		printf("get file state error:%d\n", errno);
		close(fd);
		return -1;
	}

	ptr = (char *)mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
	{
		printf("mmap failed\n");
		close(fd);
		return -1;
	}

        waitpid(child_pid, NULL, 0);
	munmap(ptr, buf.st_size);
	close(fd);
	sleep (100);
        return 0;
}

