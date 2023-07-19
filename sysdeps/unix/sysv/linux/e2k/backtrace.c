/* Return backtrace of current program state.
   Copyright (C) 2013-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by David S. Miller <davem@davemloft.net>

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <http://www.gnu.org/licenses/>.  */

#include <execinfo.h>
#include <sysdep.h>
#include <asm/e2k_syswork.h>
#include <string.h>

union pcsp_lo
{
  unsigned long long dword;
  struct
  {
    unsigned long long base : 48;
    unsigned long long dummy : 16;
  } s;
};

union pcsp_hi
{
  unsigned long long dword;
  struct
  {
    unsigned int idx;
    unsigned int dummy;
  } s;
};

#define PCSHTP_MSB 10

static unsigned long long
subsidiary (int count, void **array, int size)
{
  unsigned long long pcsp_lo, pcsp_hi;
  unsigned int pcshtp;
  __asm__ ("rrd %%pcsp.lo, %0" : "=r" (pcsp_lo));
  __asm__ ("rrd %%pcsp.hi, %0" : "=r" (pcsp_hi));
  __asm__ ("rrs %%pcshtp, %0" : "=r" (pcshtp));

  /* Take into account that only [PCSHTP_MSB..0] bits are meaningful within
     %PCSHTP according to the instruction set. While at real hosts all other
     bits usually turn out to be zero, `lsim -ch' may force them to be set to
     one  (see Bug #73907).  */
  pcshtp &= (1u << (PCSHTP_MSB + 1)) - 1;

#if 0
  printf ("\n\ncount == %d\n", count);
  printf ("pcsp_lo == 0x%llx\n", pcsp_lo);
  printf ("pcsp_hi == 0x%llx\n", pcsp_hi);
  printf ("pcshtp == 0x%x\n", pcshtp);
#endif /* 0  */

  /* `count * 32' is an amount of space required to store return info for the
     preceding subsidiaries (), `- 32' implies that %PCSHTP doesn't take %CR's
     corresponding to the current window into account, `+ 32' is required to
     store return info for backtrace (). These are all the frames we are not
     interested in.  */

  if (pcshtp > count * 32 - 32 + 32)
    return subsidiary (count + 1, array, size);
  else
    {
      int i, j;
      int res;
      unsigned long long *chain_stack;
      unsigned int pcsp_size;
      unsigned long long base;
      unsigned long long top;
      unsigned long long real_size;

      pcsp_size = (pcsp_hi & 0xffffffffULL) - (32 * count - pcshtp);
      base = pcsp_lo & 0xfffffffffff0ULL;
      top = base + pcsp_size;

      /* Always get all available frames since nobody knows how many of them
         corresponding to signal handling within the Kernel (their IP's are
         zeroed out) should be skipped. Note that only the first in a group of
         such zero frames is taken into account, it may be interpreted as a
         "signal trampoline".  */

      chain_stack = (unsigned long long *) alloca (pcsp_size);

      /* After the separation of the process kernel and user mode hardware
	 stacks there is an "unused" frame before the entry point (i.e. `_start
	 ()') at offset 0 from the base of the chain stack. The kernel could
	 completely hide it from us by setting `REAL_SIZE = PCSP_SIZE - 32' and
	 adjusting offsets in the filled in buffer (so that offset 0 in the
	 buffer matched offset 0x20 in the actual chain stack and so on) or
	 mark this "unused" frame as the "kernel" one by setting its %cr0_hi.ip
	 to 0. However, currently the kernel does not do neither the former,
	 nor the latter and prefers to leave us with junk in CHAIN_STACK[{0,1,
	 2,3}], which is almost certainly a bug. Fill in the whole CHAIN_STACK[]
	 with 0 to be on the safe side if the number of redundant uninitialized
	 frames in it increases.  */
      memset (chain_stack, 0, pcsp_size);

      res = INLINE_SYSCALL (access_hw_stacks, 5, E2K_READ_CHAIN_STACK, &top,
                            chain_stack, pcsp_size, &real_size);

      if (res < 0)
        return 0;

      /* They should be the same in fact.  */
      pcsp_size = (unsigned int) real_size;

      /* Get %cr0.hi's from the obtained copy of the chain stack.  */
      for (i = 0, j = (pcsp_size >> 3) - 3; j > 0 && i < size; j -= 4)
        {
          if (chain_stack[j])
            array[i++] = (void *) (unsigned long) chain_stack[j];
          else if (i == 0 || array[i - 1] != (void *) -1UL)
            {
              int k;
              /* Distinguish signal handling Kernel frames from the ones passing
                 control to the program's entrypoint. While the former should
                 be represented by a _single_ "signal trampoline", the latter
                 should be entirely ignored. FIXME: how should the Kernel space
                 frame at the top of the chain stack be treated taking into
                 account that this configuration seems to be impossible?  */
              for (k = j - 4; k > 0; k -= 4)
                {
                  /* Signal handling frames are usually preceded by those ones
                     belonging to user space.  */
                  if (chain_stack[k])
                    break;
                }

              /* Push a signal trampoline frame.  */
              if (k > 0)
                array[i++] = (void *) -1UL;
            }
        }

      return i;
    }
}



int
__backtrace (void **array, int size)
{
  return subsidiary (0, array, size);
}

weak_alias (__backtrace, backtrace)
libc_hidden_def (__backtrace)
