#define _GNU_SOURCE  
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE	65536

static int child_func(void *arg)
{
	printf("Child:Current Working Directory:%s\n",
			get_current_dir_name());
	chdir("/opt");
	printf("Child:Current Working Directory:%s\n",
			get_current_dir_name());
	return 0;
}

int main(int argc, char *argv[])
{
	//Allocate stack for child task
	char *stack = malloc(STACK_SIZE);
	int status;
	printf("Parent:Current Working Directory:%s\n", 
			get_current_dir_name());
	if (!stack) {
		perror("Failed to allocate memory\n");
		exit(1);
	}
	
	if (clone(child_func, stack + STACK_SIZE, CLONE_FS | SIGCHLD, NULL) == -1) {
		perror("clone");
		exit(1);
	}
	if (wait(&status) == -1) {
		perror("Wait");
		exit(1);
	}	
	printf("Child exited with status:%d\t cwd:%s\n",
			status, get_current_dir_name());
	return 0;
}
