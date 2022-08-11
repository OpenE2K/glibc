#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

static int
exec_wait (const char *path, const char *av[])
{
	pid_t   pid = vfork ();

	if (pid < 0)
		return 1;
	if (!pid)
	{
		execv (path, (char *const *) av);
		_exit (1);
	} else
	{
		int     status = 0;

		if (waitpid (pid, &status, 0) != pid || !WIFEXITED (status))
			return 1;
		return WEXITSTATUS (status);
	}
}

int
main (int ac, const char *av[])
{
	const char *fixpost_args[] = { "glibc_fix_post", 0 };
	const char *ldconfig_args[] = { "ldconfig", 0 };
	const char *p;

	if ((p = getenv ("RPM_INSTALL_ARG1")) &&
	    *p && (*p != '0') && (*p != '1'))
		(void) exec_wait ("/sbin/glibc_fix_post", fixpost_args);

	return exec_wait ("/sbin/post_ldconfig", ldconfig_args);
}
