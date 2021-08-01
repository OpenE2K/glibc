/* Definition of `struct stat' used in the kernel */

struct timespec_kernel
{
    long long int tv_sec;		/* Seconds.  */
    unsigned long long int tv_nsec;		/* Nanoseconds.  */
};

struct kernel_stat
  {
    unsigned long long int st_dev;
    unsigned long long int st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned long long int st_rdev;
    unsigned long long int st_size;
    unsigned long long int st_blksize;
    unsigned long long int st_blocks;
    struct timespec_kernel st_atim;
    struct timespec_kernel st_mtim;
    struct timespec_kernel st_ctim;
  };

#define _HAVE_STAT___UNUSED1
#define _HAVE_STAT64___UNUSED1
#define _HAVE_STAT_NSEC
#define _HAVE_STAT64_NSEC

/* We have implicitly used these values for years.  */
#define XSTAT_IS_XSTAT64	0
#define STATFS_IS_STATFS64	0
