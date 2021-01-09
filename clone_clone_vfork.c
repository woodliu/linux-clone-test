#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static char child_stack[1048576];

static int child_fn() {
        printf("child process  parent PID: %ld\n", (long)getpid());
	char *argv[ ]={"sleep", "10",  NULL};
	char *envp[ ]={"PATH=/usr/bin", NULL};
	execve("/usr/bin/sleep", argv, envp);
        return 0;
}

int main() {
        printf("calling proecess PID: %ld\n", (long)getpid());
        pid_t child_pid = clone(child_fn, child_stack+1048576, CLONE_VFORK | SIGCHLD, NULL);
        printf("clone() = %ld\n", (long)child_pid);

        waitpid(child_pid, NULL, 0);
        return 0;
}

