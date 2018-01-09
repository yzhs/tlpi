#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int
my_dup(int fd)
{
	return fcntl(fd, F_DUPFD, 0);
}

static int
is_valid(int fd)
{
	return fcntl(fd, F_GETFL) >= 0;
}

int
my_dup2(int fd, int fd2)
{
	if (fd == fd2) {
		if (!is_valid(fd)) {
			errno = EBADF;
			return -1;
		}
		return fd;
	}

	if (is_valid(fd2) && -1 == close(fd2)) {
		err(1, "close");
	}
	return fcntl(fd, F_DUPFD, fd2);
}

int main(void)
{
	assert(my_dup2(3, 3) == -1);
	assert(errno == EBADF);

	assert(my_dup2(2, 2) == 2);

	assert(my_dup2(1, 3) == 3);
	assert(my_dup2(4, 7) == -1);

	return 0;
}
