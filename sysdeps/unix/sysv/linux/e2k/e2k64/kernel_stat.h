/* Definition of `struct stat' used in the kernel */
struct kernel_stat
  {
    unsigned int st_dev;
    unsigned long int st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned int st_rdev;
    long int st_size;
    long int st_blksize;
    long int st_blocks;

    long int st_atime_sec;
    unsigned long int st_atime_nsec;
    
    long int st_mtime_sec;
    unsigned long int st_mtime_nsec;

    long int st_ctime_sec;
    unsigned long int st_ctime_nsec;
  };

/* Definition of `struct stat64' used in the kernel.  */
struct kernel_stat64
  {
    unsigned long st_dev;
    unsigned long st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned long st_rdev;
    unsigned long st_size;
    unsigned int st_blksize;
    unsigned int __unused1;
    unsigned long st_blocks;
    int st_atime_sec;
    unsigned int st_atime_nsec;
    int	st_mtime_sec;
    unsigned int st_mtime_nsec;
    int st_ctime_sec;
    unsigned int st_ctime_nsec;
  };

#define XSTAT_IS_XSTAT64 1
