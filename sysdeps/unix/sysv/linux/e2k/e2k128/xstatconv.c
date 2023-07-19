#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <kernel_stat.h>
#include <string.h>
#include <kernel-features.h>

int
__xstat_conv (int vers, struct kernel_stat *kbuf, void *ubuf)
{
  switch (vers)
    {
    case _STAT_VER_LINUX:
      {
	struct stat *buf = ubuf;

	/* Convert to current kernel version of `struct stat'.  */
	buf->st_dev = kbuf->st_dev;
	buf->st_ino = kbuf->st_ino;
	buf->st_mode = kbuf->st_mode;
	buf->st_nlink = kbuf->st_nlink;
	buf->st_uid = kbuf->st_uid;
	buf->st_gid = kbuf->st_gid;
	buf->st_rdev = kbuf->st_rdev;
	buf->st_size = kbuf->st_size;
	buf->st_blksize = kbuf->st_blksize;
	buf->st_blocks = kbuf->st_blocks;

	buf->st_atim.tv_sec = kbuf->st_atime_sec;
	buf->st_atim.tv_nsec = kbuf->st_atime_nsec;

	buf->st_mtim.tv_sec = kbuf->st_mtime_sec;
	buf->st_mtim.tv_nsec = kbuf->st_mtime_nsec;

	buf->st_ctim.tv_sec = kbuf->st_ctime_sec;
	buf->st_ctim.tv_nsec = kbuf->st_ctime_nsec;

	buf->__glibc_reserved1 = 0;
	buf->__pad1 = 0;
	buf->__pad2 = 0;
      }
      break;

    default:
      __set_errno (EINVAL);
      return -1;
    }

  return 0;
}
