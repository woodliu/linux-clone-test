#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static char child_stack[1048576];

static int child_fn() {
	printf("PID: %ld\n", (long)getpid());
	sleep(100);
	return 0;
}

int main() {
	pid_t child_pid = clone(child_fn, child_stack+1048576, CLONE_NEWPID | SIGCHLD, NULL);
	printf("clone() = %ld\n", (long)child_pid);

	waitpid(child_pid, NULL, 0);
	return 0;
}
