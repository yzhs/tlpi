/*
 * Copyright © 2018 Colin Benner
 * Copyright © 2011 Michael Kerrisk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _FILE_OFFSET_BITS 64
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	int fd;
	off_t off;

	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
		err(1, "%s pathname offset\n", argv[0]);
	}

	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		err(1, "open");
	}

	off = atoll(argv[2]);
	if (lseek(fd, off, SEEK_SET) == -1) {
		err(1, "lseek");
	}

	if (write(fd, "test", 4) == -1) {
		err(1, "write");
	}

	return 0;
}
