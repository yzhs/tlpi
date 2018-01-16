#define _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <unistd.h>

void
printids()
{
	uid_t uid, euid, suid;
	if (getresuid(&uid, &euid, &suid) == -1) {
		err(1, "getresuid");
	}
	printf("real=%d, effective=%d, saved=%d\n", uid, euid, suid);
}

int
main(int argc, char *argv[])
{
	if (argc == 1) {
		goto error;
	}

	switch (argv[1][0]) {
		case '1':
			printids();
			setuid(2000);
			break;
		case '2':
			printids();
			setreuid(-1, 2000);
			break;
		case '3':
			printids();
			seteuid(2000);
			break;
		case '4':
			printids();
			setresuid(-1, 2000, 3000);
			break;
		default:
			goto error;
	}

	printids();

	return 0;

error:
	printf("Usage: %s [1234]\n", argv[0]);
	return 1;
}
