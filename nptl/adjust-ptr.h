#if ! defined __ptr128__
# define ADJUST_PTR(ptr, op, num) ((uintptr_t) (ptr) op (num))
#else /* defined __ptr128__  */
# define ADJUST_PTR(ptr, op, num)		\
  ({						\
    const char *__ptr = (const char *) (ptr);	\
    uintptr_t __num = (uintptr_t) (num);	\
    uintptr_t __delta = (uintptr_t) __ptr;	\
    __delta = (__delta op __num) - __delta;	\
    __ptr + (long) __delta;			\
  })
#endif /* defined __ptr128__  */
