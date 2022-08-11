/* gcc -s -static ${CFLAGS} -Wl,-zmuldefs glibc_preinstall.c */

#include <unistd.h>
#include <signal.h>

static void print_msg(const char *const msg)
{
	size_t len;
	for (len = 0; msg[len]; ++len)
		;
	write(2, msg, len);
}

static void stop_parent(void)
{
	pid_t pid = getppid();
	if (pid < 100)
		return;

	print_msg("Sending SIGSTOP signal to parent process.\n");
	(void) kill(pid, SIGSTOP);
}

void __libc_fatal(const char *const msg)
{
	print_msg(msg);
	stop_parent();
	_exit(1);
}

int main(void)
{
	return 0;
}
