static inline void
__cp_kstat_stat64_t64 (const struct kernel_stat *kst, struct __stat64_t64 *st)
{
  st->st_dev = kst->st_dev;
  st->st_ino = kst->st_ino;
  st->st_mode = kst->st_mode;
  st->st_nlink = kst->st_nlink;
  st->st_uid = kst->st_uid;
  st->st_gid = kst->st_gid;
  st->st_rdev = kst->st_rdev;
  st->st_size = kst->st_size;
  st->st_blksize = kst->st_blksize;
  st->st_blocks = kst->st_blocks;

  st->st_atim.tv_sec = kst->st_atime_sec;
  st->st_atim.tv_nsec = kst->st_atime_nsec;

  st->st_mtim.tv_sec = kst->st_mtime_sec;
  st->st_mtim.tv_nsec = kst->st_mtime_nsec;

  st->st_ctim.tv_sec = kst->st_ctime_sec;
  st->st_ctim.tv_nsec = kst->st_ctime_nsec;

  st->__glibc_reserved1 = 0;
  st->__pad1 = 0;
  //  st->__pad2 = 0;
}
