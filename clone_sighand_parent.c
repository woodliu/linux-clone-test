/* Example of using sigaction() to setup a signal handler with 3 arguments
 * including siginfo_t.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static char child_stack[1048576];

static int child_fn() {
	printf("PID: %ld\n", (long)getpid());
	sigset_t new_set;
	sigemptyset( &new_set );
	sigaddset( &new_set, SIGINT );
	sigprocmask(SIG_BLOCK, &new_set, NULL);
	sleep (100);
	return 0;
}


static void hdl (int sig, siginfo_t *siginfo, void *context)
{
        printf ("Sending PID: %ld, UID: %ld\n",
                        (long)siginfo->si_pid, (long)siginfo->si_uid);
}

int main() {
	struct sigaction act;
        
        pid_t child_pid = clone(child_fn, child_stack+1048576, CLONE_VM | CLONE_SIGHAND | SIGCHLD, NULL);
	printf("clone() = %ld\n", (long)child_pid);
        
        memset (&act, '\0', sizeof(act));

        /* Use the sa_sigaction field because the handles has two additional parameters */
        act.sa_sigaction = &hdl;

        /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
        act.sa_flags = SA_SIGINFO;

        if (sigaction(SIGINT, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
        }

	waitpid(child_pid, NULL, 0);
	printf("child terminated!\n");
	sleep (100);
	return 0;
}
