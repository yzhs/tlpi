#include <err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

enum { BUF_SIZE = 512 };

bool
is_zero(const char buf[], const size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		if (buf[i] != 0) {
			return false;
		}
	}
	return true;
}


int
main(int argc, char *argv[])
{
	if (argc != 3) {
		errx(1, "Usage: %s SOURCE DEST", argv[0]);
	}

	int source = open(argv[1], O_RDONLY);
	if (source == -1) {
		err(1, "Failed to open source '%s' for reading", argv[1]);
	}

	int dest = creat(argv[2], 0644);
	if (dest == -1) {
		err(1, "Failed to open destination '%s' for writing", argv[2]);
	}

	char buf[BUF_SIZE];
	ssize_t len;
	bool ends_with_hole = false;
	while (0 < (len = read(source, buf, sizeof(buf)))) {
		if (is_zero(buf, len)) {
			if (-1 == lseek(dest, len, SEEK_CUR)) {
				err(1, "lseek failed");
			}
			ends_with_hole = true;
		} else {
			ssize_t len_written = write(dest, buf, len);
			if (len_written < len) {
				err(1, "Failed to write %llu bytes to destination '%s'",
						(unsigned long long)len, argv[2]);
			}
			ends_with_hole = false;
		}
	}

	// Write a byte at the end so the destination gets a final hole as well.
	if (ends_with_hole) {
		if (-1 == lseek(dest, -1, SEEK_CUR)) {
			err(1, "lseek failed");
		}
		char zero = 0;
		if (write(dest, &zero, 1) <= 0) {
			err(1, "Failed to write final byte");
		}
	}

	if (len == -1) {
		err(1, "Failed to read from source");
	}

	if (close(dest) == -1) {
		err(1, "Failed to close destination");
	}
	if (close(source) == -1) {
		err(1, "Failed to close source");
	}

	return 0;
}
