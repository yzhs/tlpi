#define _DEFAULT_SOURCE

#include <err.h>
#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	switch (vfork()) {
		case -1:
			err(1, "vfork");

		case 0:
			if (close(1) == -1) {
				err(1, "close");
			}
			_exit(0);

		default:
			sleep(5);
			if (puts("Parent can still write to stdout!") < 0) {
				err(1, "puts parent");
			}
	}

	return 0;
}
