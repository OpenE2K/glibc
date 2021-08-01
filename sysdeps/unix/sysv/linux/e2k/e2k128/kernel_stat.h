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
    long long int st_atime_sec;
    unsigned long long int st_atime_nsec;
    long long int st_mtime_sec;
    unsigned long long int st_mtime_nsec;
    long long int st_ctime_sec;
    unsigned long long int st_ctime_nsec;
};

#define XSTAT_IS_XSTAT64 1
