// TODO implement setenv() and unsetenv() using getenv() and putenv(), and,
// where necessary, by directly manipulating `environ`. If there are multiple
// definitions of a variable, unsetenv() should remove them all.
#define _XOPEN_SOURCE

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern char **environ;

static int
is_prefix(const char *haystack, const char *needle)
{
	return strstr(haystack, needle) == haystack;
}

static void
remove_copies(const char *name)
{
	for (char **e = environ; *e; e++) {
		if (!is_prefix(*e, name) || (*e)[strlen(name)] != '=') {
			continue;
		}
		for (char **env = e; *env; env++) {
			*env = *(env+1);
		}
	}
}

int
my_unsetenv(const char *name)
{
	if (!name || strlen(name) == 0 || strchr(name, '=') != NULL) {
		errno = EINVAL;
		return -1;
	}
	remove_copies(name);
	return 0;
}

int
my_setenv(const char *name, const char *value, int overwrite)
{
	if (!name || strlen(name) == 0 || strchr(name, '=') != NULL) {
		errno = EINVAL;
		return -1;
	}
	char *existing_value = getenv(name);
	if (!existing_value || overwrite) {
		size_t len = strlen(name) + sizeof('=') + strlen(value) + sizeof('\0');
		char *new_def = malloc(len);
		if (!new_def) {
			return -1;
		}

		strncpy(new_def, name, len);
		new_def[strlen(name)] = '=';
		strncpy(&new_def[strlen(name)+1], value, len);

		if (0 != putenv(new_def)) {
			return -1;
		}
	}

	return 0;
}

int main(void)
{
	puts("unset");
	assert(my_unsetenv("XDG_SESSION_ID") == 0);
	system("/bin/env | /bin/grep -s XDG_SESSION_ID"); // XXX Unsafe

	puts("\nset non-existing");
	assert(my_setenv("XDG_SESSION_ID", "abc", 0) == 0);
	system("/bin/env | /bin/grep -s XDG_SESSION_ID"); // XXX Unsafe

	puts("\nset existing, no overwrite");
	assert(my_setenv("XDG_SESSION_ID", "def", 0) == 0);
	system("/bin/env | /bin/grep -s XDG_SESSION_ID"); // XXX Unsafe

	puts("\nset existing, overwrite");
	assert(my_setenv("XDG_SESSION_ID", "def", 1) == 0);
	system("/bin/env | /bin/grep -s XDG_SESSION_ID"); // XXX Unsafe

	return 0;
}
