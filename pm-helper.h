#define puttag(a, b) __asm__ ("puttagd %1, %2, %0"	\
			      : "=r" (a)		\
			      : "r" (a), "ri" (b));

#define gettag(a)					\
  ({							\
    int res;						\
    __asm__ ("gettagd %1, %0" : "=r" (res) : "r" (a));	\
    res;						\
  })

static void
print_pm_ptr (const char *name, void *arg)
{
  long *arr = (long *) &arg;
  int t[2];

  t[0] = gettag (arr[0]);
  t[1] = gettag (arr[1]);
  puttag (arr[0], 0);
  puttag (arr[1], 0);

  printf ("%s == <0x%x> 0x%lx : <0x%x> 0x%lx\n",
	  name, t[0], arr[0], t[1], arr[1]);
}
