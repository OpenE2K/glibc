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

static __attribute__ ((noinline)) int
subsidiary (void **array, int size)
{
  int res;
  size_t chain_stack_size;
  ssize_t j;
  int i;
  unsigned long long *chain_stack;
  unsigned long long bottom = 0;
  unsigned long long ra;

  res = INLINE_SYSCALL (access_hw_stacks, 5, E2K_GET_CHAIN_STACK_SIZE, NULL,
			NULL, 0, &chain_stack_size);

  if (res != 0)
    return 0;

  /* Always get all available frames since nobody knows how many of them
     corresponding to signal handling within the Kernel (their IP's are
     zeroed out) should be skipped. Note that only the first in a group of
     such zero frames is taken into account, it may be interpreted as a
     "signal trampoline".  */
  chain_stack = (unsigned long long *) alloca (chain_stack_size);

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
  memset (chain_stack, 0, chain_stack_size);

  res = INLINE_SYSCALL (access_hw_stacks, 5, E2K_READ_CHAIN_STACK_EX, &bottom,
			chain_stack, chain_stack_size, NULL);

  if (res != 0)
    return 0;

  ra = ((unsigned long long) (unsigned long)
	__builtin_extract_return_addr (__builtin_return_address (0)));

  for (j = (chain_stack_size >> 3) - 3; j > 0; j -= 4)
    {
      /* Have we come across backtrace () frame?  */
      if (chain_stack[j] == ra)
	{
	  /* Skip it and proceed to filling in ARRAY[] with preceding
	     frames.  */
	  j -= 4;
	  break;
	}
    }

  /* Get %cr0.hi's from the obtained copy of the chain stack.  */
  for (i = 0; j > 0 && i < size; j -= 4)
    {
      if (chain_stack[j])
	array[i++] = (void *) (unsigned long) chain_stack[j];
      else if (i == 0 || array[i - 1] != (void *) -1UL)
	{
	  ssize_t k;
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



int
__backtrace (void **array, int size)
{
  return subsidiary (array, size);
}

weak_alias (__backtrace, backtrace)
libc_hidden_def (__backtrace)
