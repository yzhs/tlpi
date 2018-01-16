#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	int wstatus;
	switch (fork()) {
		case -1:
			err(1, "fork");

		case 0:
			_exit(-1);

		default:
			if (wait(&wstatus) == -1) {
				err(1, "wait");
			}
			printf("%d\n", wstatus);
			printf("%d\n", WEXITSTATUS(wstatus));
	}

	return 0;
}
