#define _POSIX_C_SOURCE 2

#include <errno.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

enum { BUF_SIZE = 1024 };

int
main(int argc, char *argv[])
{
	char buf[BUF_SIZE];
	bool append = false;

	int opt;
	while ((opt = getopt(argc, argv, "+a")) != -1) {
		switch (opt) {
			case 'a':
				append = true;
				break;
			default:
				errx(1, "Usage: %s [-a] [FILE]...", argv[0]);
		}
	}
	argc -= optind;
	argv += optind;

	// Open output files
	int nfiles = argc;
	int *files;
	if (NULL == (files = calloc(nfiles + 1, sizeof(int)))) {
		err(1, "Failed to allocate memory for file handles.");
	}

	int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);

	files[0] = 1; // stdout
	for (int i = 0; i < nfiles; ++i) {
		if (-1 == (files[i+1] = open(argv[i], flags, 0644))) {
			err(1, "Failed to open file '%s' for writing", argv[i]);
		}
	}

	ssize_t len;
	while (0 < (len = read(0, buf, sizeof(buf)))) {
		for (int i = 0; i < nfiles+1; ++i) {
			size_t remaining = len;
			while (remaining) {
				ssize_t bytes_written;
				if (-1 == (bytes_written = write(files[i], buf, (size_t)len))) {
					err(1, "Failed to write to file number '%d'", i);
				}
				remaining -= (size_t)bytes_written;
			}
		}
	}
	if (len == -1) {
		err(1, "Failed to write to file");
	}

	for (int i = 1; i < nfiles; ++i) {
		if (-1 == close(files[i])) {
			err(1, "Failed to close file '%s'", argv[i]);
		}
	}

	free(files);

	return 0;
}
