#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/auxv.h>
#include <not-cancel.h>

static long linux_sysconf (int name);

enum cache_type
{
   ICACHE,
   DCACHE,
   CACHE
};

enum cache_quantity
{
   SIZE,
   ASSOC,
   LINESIZE
};



static inline long
cache_quantity (int level, enum cache_type type, enum cache_quantity quant)
{
  int i;
  int cpu = sched_getcpu ();

  for (i = 0; i < 4; i++)
    {
      int fd;
      /* When reserving space believe that 4 digits are enough to represent a
	 CPU id. Also make use of the fact that "ways_of_associativity" is the
	 longest possible name of a quantity.  */
      char fname[]
	= "/sys/devices/system/cpu/cpuWXYZ/cache/indexZ/ways_of_associativity";

      if (__snprintf (fname, sizeof (fname),
		      "/sys/devices/system/cpu/cpu%d/cache/index%d/level",
		      cpu, i) >= sizeof (fname))
	break;

      fd = __open_nocancel (fname, O_RDONLY);
      if (fd == -1)
	continue;

      /* This should be more than enough as the file is expected to contain a
	 single digit and the newline.  */
      char level_str[3];
      ssize_t n;
      n = TEMP_FAILURE_RETRY (__read_nocancel (fd, level_str, 3));
      __close_nocancel_nostatus (fd);
      if (n != 2 || level_str[1] != '\n' || level_str[0] != '0' + (char) level)
	continue;

      if (__snprintf (fname, sizeof (fname),
		      "/sys/devices/system/cpu/cpu%d/cache/index%d/type",
		      cpu, i) >= sizeof (fname))
	break;

      fd = __open_nocancel (fname, O_RDONLY);
      if (fd == -1)
	continue;
      /* "Instruction" should be the longest supported type, 'x' just reserves
	 an extra char.  */
      char type_str[] = "Instructionx\n";
      n = TEMP_FAILURE_RETRY (__read_nocancel (fd, type_str,
					       sizeof (type_str)));
      __close_nocancel_nostatus (fd);

      const char *expected_type = (type == ICACHE
				   ? "Instruction"
				   : (type == DCACHE ? "Data" : "Unified"));
      if (n == sizeof (type_str)
	  || type_str[n - 1] != '\n'
	  || strncmp (type_str, expected_type, n - 1) != 0)
	continue;

      const char *quant_name = (quant == SIZE
				? "size"
				: (quant == ASSOC
				   ? "ways_of_associativity"
				   : "coherency_line_size"));

      if (__snprintf (fname, sizeof (fname),
		      "/sys/devices/system/cpu/cpu%d/cache/index%d/%s",
		      cpu, i, quant_name) >= sizeof (fname))
	break;

      fd = __open_nocancel (fname, O_RDONLY);
      if (fd == -1)
	continue;

      /* This should be more than enough as the file contains a single integer
	 and maybe a trailing K.  */
      char buf[32];

      n = TEMP_FAILURE_RETRY (__read_nocancel (fd, buf, sizeof (buf)));
      __close_nocancel_nostatus (fd);

      if (n == sizeof (buf) || buf[n - 1] != '\n')
	continue;

      buf[n - 1] = '\0';
      char *endp;
      long int res = strtol (buf, &endp, 10);
      if (endp != buf)
	{
	  if (*endp == '\0')
	    return res;
	  else if (quant == SIZE && *endp == 'K')
	    return res * 1024;
	}
    }

  /* 0 stands for an undetermined cache quantity.  */
  return 0;
}


/* Get the value of the system variable NAME.  */
long int
__sysconf (int name)
{
  switch (name)
    {
      case _SC_LEVEL1_ICACHE_SIZE:
	return cache_quantity (1, ICACHE, SIZE);
      case _SC_LEVEL1_ICACHE_ASSOC:
	return cache_quantity (1, ICACHE, ASSOC);
      case _SC_LEVEL1_ICACHE_LINESIZE:
	return cache_quantity (1, ICACHE, LINESIZE);
      case _SC_LEVEL1_DCACHE_SIZE:
	return cache_quantity (1, DCACHE, SIZE);
      case _SC_LEVEL1_DCACHE_ASSOC:
	return cache_quantity (1, DCACHE, ASSOC);
      case _SC_LEVEL1_DCACHE_LINESIZE:
	return cache_quantity (1, DCACHE, LINESIZE);
      case _SC_LEVEL2_CACHE_SIZE:
	return cache_quantity (2, CACHE, SIZE);
      case _SC_LEVEL2_CACHE_ASSOC:
	return cache_quantity (2, CACHE, ASSOC);
      case _SC_LEVEL2_CACHE_LINESIZE:
	return cache_quantity (2, CACHE, LINESIZE);
      case _SC_LEVEL3_CACHE_SIZE:
	return cache_quantity (3, CACHE, SIZE);
      case _SC_LEVEL3_CACHE_ASSOC:
	return cache_quantity (3, CACHE, ASSOC);
      case _SC_LEVEL3_CACHE_LINESIZE:
	return cache_quantity (3, CACHE, LINESIZE);

      default:
	return linux_sysconf (name);
    }
}

/* Now the generic Linux version.  */
#undef __sysconf
#define __sysconf static linux_sysconf
#include "../sysconf.c"
