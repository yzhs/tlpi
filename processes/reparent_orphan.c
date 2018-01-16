#define _POSIX_C_SOURCE 199506L

#include <sys/types.h>
#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void
handler(int signum)
{
	(void)signum;
}

int
main(void)
{
	pid_t ppid;
	sigset_t sigmask;
	int sig;
	if (sigemptyset(&sigmask) == -1) {
		err(1, "sigemptyset");
	}
	if (sigaddset(&sigmask, SIGUSR1) == -1) {
		err(1, "sigaddset");
	}
	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		err(1, "sigaction");
	}

	pid_t child;
	switch ((child = fork())) {
		case -1:
			err(1, "fork");
		case 0:
			ppid = getppid();
			printf("Old parent id: %lld\n", (long long)ppid);
			kill(ppid, SIGUSR1);

			if (sigwait(&sigmask, &sig) == -1) {
				err(1, "sigsuspend");
			}
			sleep(1);
			ppid = getppid();
			printf("%lld\n", (long long)ppid);
			break;
		default:
			if (sigwait(&sigmask, &sig) == -1) {
				err(1, "sigsuspend");
			}
			puts("Parent exiting...");
			kill(child, SIGUSR1);
			_exit(0);

	}
	return 0;
}
