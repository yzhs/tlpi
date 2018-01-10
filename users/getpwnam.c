#define _XOPEN_SOURCE 500
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>

struct passwd *
my_getpwnam(const char *name) {
	struct passwd *pwd;
	int old_errno = errno;

	setpwent();

	errno = 0;
	while ((pwd = getpwent())) {
		if (errno) {
			return NULL;
		}
		if (strcmp(pwd->pw_name, name) == 0) {
			endpwent();
			return pwd;
		}
		errno = 0;
	}

	endpwent();
	errno = old_errno;
	return NULL;
}

static int
equal(const struct passwd * const pwd1, const struct passwd * const pwd2)
{
	if ((pwd1 == NULL) != (pwd2 == NULL)) {
		return 0;
	}
	if (!pwd1) {
		return 1;
	}

	return pwd1->pw_uid == pwd2->pw_uid &&
		pwd1->pw_gid == pwd2->pw_gid &&
		strcmp(pwd1->pw_name, pwd2->pw_name) == 0 &&
		strcmp(pwd1->pw_passwd, pwd2->pw_passwd) == 0 &&
		strcmp(pwd1->pw_gecos, pwd2->pw_gecos) == 0 &&
		strcmp(pwd1->pw_dir, pwd2->pw_dir) == 0 &&
		strcmp(pwd1->pw_shell, pwd2->pw_shell) == 0;
}

int
main(void)
{
	char *name = getenv("LOGNAME");
	name = name ? name : "root";
	struct passwd *pwd = getpwnam(name);
	struct passwd *copy = malloc(sizeof(struct passwd));
	if (!copy) {
		err(1, "malloc");
	}
	memcpy(copy, pwd, sizeof(struct passwd));
	pwd = my_getpwnam(name);
	assert(equal(copy, pwd));
	return 0;
}
