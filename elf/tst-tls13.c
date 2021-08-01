/* Check unloading modules with data in static TLS block.  */
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if ! defined __ptr128__
# define COUNT 1000
#else /* defined __ptr128__  */
# define COUNT 10
#endif /* defined __ptr128__  */



static int
do_test (void)
{
  for (int i = 0; i < COUNT;)
    {
      printf ("round %d\n",++i);

      void *h = dlopen ("$ORIGIN/tst-tlsmod13a.so", RTLD_LAZY);
      if (h == NULL)
	{
	  printf ("cannot load: %s\n", dlerror ());
	  exit (1);
	}

      dlclose (h);
    }

  return 0;
}

#include <support/test-driver.c>
