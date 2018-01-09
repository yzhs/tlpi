#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		errx(1, "Usage: %s FILE", argv[0]);
	}

	int fd = open(argv[1], O_WRONLY | O_APPEND);
	if (fd == -1) {
		err(1, "open");
	}

	if (lseek(fd, 0, SEEK_SET) == -1) {
		err(1, "lseek");
	}

	/* When O_APPEND is specified, all writes go to the end of the file. It
	 * does not matter whether or not we seek to a certain position. */
	char str[] = "Foobar";
	if (sizeof(str) != write(fd, str, sizeof(str))) {
		err(1, "write");
	}

	if (-1 == close(fd)) {
		err(1, "close");
	}

	return 0;
}
