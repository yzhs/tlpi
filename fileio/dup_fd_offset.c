#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int fd = creat("foo.txt", 0644);
	if (-1 == fd) {
		err(1, "creat");
	}

	int fd2 = dup(fd);
	char str[] = "test\n123\nabc\n";
	if (sizeof(str) != write(fd, str, sizeof(str))) {
		err(1, "write");
	}
	if (-1 == lseek(fd2, 0, SEEK_SET)) {
		err(1, "lseek");
	}
	if (4 != write(fd, "1234", 4)) {
		err(1, "write");
	}

	return 0;
}
