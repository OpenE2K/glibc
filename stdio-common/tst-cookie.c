#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>


#define THE_COOKIE ((void *) 0xdeadbeeful)

static int errors;


static int cookieread_called;
static ssize_t
cookieread (void *cookie, char *buf, size_t count)
{
#if defined __ptr128__
  size_t i;
#endif /* defined __ptr128__  */

  printf ("`%s' called with cookie %#lx\n", __FUNCTION__,
	  (unsigned long int) cookie);
  if ((unsigned int) cookie != (unsigned int) THE_COOKIE)
    ++errors;
  cookieread_called = 1;

#if defined __ptr128__
  /* Generic glibc code is likely to touch the contents of the returned buffer
     which may obviously result in `exc_illegal_operand' in PM if you don't
     take care of actually setting it up.  */
  for (i = 0; i < size; i++)
    buf[i] = (char) i;
#endif /* defined __ptr128__  */

  return 42;
}


static int cookiewrite_called;
static ssize_t
cookiewrite (void *cookie, const char *buf, size_t count)
{
  printf ("`%s' called with cookie %#lx\n", __FUNCTION__,
	  (unsigned long int) cookie);
  if (cookie != THE_COOKIE)
    ++errors;
  cookiewrite_called = 1;
  return 43;
}


static int cookieseek_called;
static int
cookieseek (void *cookie, off64_t *offset, int whence)
{
  printf ("`%s' called with cookie %#lx\n", __FUNCTION__,
	  (unsigned long int) cookie);
  if (cookie != THE_COOKIE)
    ++errors;
  cookieseek_called = 1;
  return 44;
}


static int cookieclose_called;
static int
cookieclose (void *cookie)
{
  printf ("`%s' called with cookie %#lx\n", __FUNCTION__,
	  (unsigned long int) cookie);
  if (cookie != THE_COOKIE)
    ++errors;
  cookieclose_called = 1;
  return 45;
}


static int
do_test (void)
{
  cookie_io_functions_t fcts;
  char buf[1];
  FILE *f;

  fcts.read = cookieread;
  fcts.seek = cookieseek;
  fcts.close = cookieclose;
  fcts.write = cookiewrite;

  f = fopencookie (THE_COOKIE, "r+", fcts);

  fread (buf, 1, 1, f);
  fwrite (buf, 1, 1, f);
  fseek (f, 0, SEEK_CUR);
  fclose (f);

  if (cookieread_called == 0
      || cookiewrite_called == 0
      || cookieseek_called == 0
      || cookieclose_called == 0)
    ++errors;

  return errors != 0;
}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
