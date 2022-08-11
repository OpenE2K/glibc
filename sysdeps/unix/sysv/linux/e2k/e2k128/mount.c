#include <sysdep.h>
#include <sys/mount.h>

/* This `extern' and subsequent `libc_hidden_proto's would probably be placed
   into `include/sys/mount.h' if it existed. Presumably it's missing because
   originally glibc doesn't allow for `mount ()' to be implemented in C.  */
extern int __mount (const char *source, const char *target,
                    const char *filesystemtype,
                    unsigned long mountflags, const void *data);

libc_hidden_proto (mount)
libc_hidden_proto (__mount)

int
__mount (const char *source, const char *target, const char *filesystemtype,
         unsigned long mountflags, const void *data)
{
#if ! defined __ptr128_new_abi__

  struct
  {
    const char *a;
    const char *b;
    const char *c;
    unsigned long d;
    const void *e;
  }
  args = {source, target, filesystemtype, mountflags, data};

  return INLINE_SYSCALL (mount, 1, &args);

#else /* defined __ptr128_new_abi__  */

  return INLINE_SYSCALL (mount, 5, source, target, filesystemtype,
			 mountflags, data);

#endif /* defined __ptr128_new_abi__  */
}

/* FIXME: this magic has stupidly been borrowed from many other files like
   `sysdeps/unix/sysv/linux/read.c'. Note, however, that it makes `__GI_mount'
   weak rather than `mount' (due to asm redirection in the above `libc_hidden_
   proto (mount)' in fact, whereas when compiling an automatically generated
   assembler file it's definitely `mount' that becomes weak.  */
weak_alias (__mount, mount)
libc_hidden_def (__mount)
libc_hidden_def (mount)
