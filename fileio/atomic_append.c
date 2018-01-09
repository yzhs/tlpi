/*
 * Without using O_APPEND (relying on lseek instead), this programme has a race
 * condition: When one instance writes to the output file after the other did
 * an lseek(), but before it did the corresponding write(), both instances will
 * write the same byte, i.e. one will overwrite one of the bytes written by the
 * other. Therefore, some bytes will probably be missing from the output file.
 */

#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	if (argc < 3 || argc > 4 || (argc == 4 && (argv[3][0] != 'x' || argv[3][1] != '\0'))) {
		errx(1, "Usage: %s filename num-bytes [x]", argv[0]);
	}

	int x = argc == 4;

	int fd = open(argv[1], O_WRONLY | O_CREAT | (x ? O_APPEND : 0), 0644);
	if (fd == -1) {
		err(1, "open");
	}

	int n = atoi(argv[2]);
	for (int i = 0; i < n; ++i) {
		if (!x && -1 == lseek(fd, 0, SEEK_END)) {
			err(1, "lseek");
		}
		if (1 != write(fd, "a", 1)) {
			err(1, "write");
		}
	}

	if (-1 == close(fd)) {
		err(1, "close");
	}

	return 0;
}
