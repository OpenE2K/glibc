/* Copyright (C) 1994-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Joel Sherrill (jsherril@redstone-emh2.army.mil),
     On-Line Applications Research Corporation.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
do_test (void)
{
  int failures = 0;
  int i;

#if defined (__LCC__)
#define try(_name, _param, _value, _expected)                           \
  ({                                                                    \
    if (_value != _expected)                                            \
      {                                                                 \
        printf ("%s(%#llx) expected %d got %d\n",                       \
                _name, _param, _expected, _value);                      \
        ++failures;                                                     \
      }                                                                 \
    else                                                                \
      printf ("%s(%#llx) as expected %d\n", _name, _param, _value);     \
  })
#else /* __LCC__ */
  auto void try (const char *name, long long int param, int value,
		 int expected);

  void try (const char *name, long long int param, int value, int expected)
    {
      if (value != expected)
	{
	  printf ("%s(%#llx) expected %d got %d\n",
		  name, param, expected, value);
	  ++failures;
	}
      else
	printf ("%s(%#llx) as expected %d\n", name, param, value);
    }
#endif /* __LCC__ */

#define TEST(fct, type) \
  try (#fct, 0, fct ((type) 0), 0);					      \
  for (i=0 ; i < 8 * sizeof (type); i++)				      \
    try (#fct, 1ll << i, fct (((type) 1) << i), i + 1);			      \
  for (i=0 ; i < 8 * sizeof (type) ; i++)				      \
    try (#fct, (~((type) 0) >> i) << i, fct ((~((type) 0) >> i) << i), i + 1);\
  try (#fct, 0x80008000, fct ((type) 0x80008000), 16)

  TEST (ffs, int);
  TEST (ffsl, long int);
  TEST (ffsll, long long int);

  if (failures)
    printf ("Test FAILED!  %d failure%s.\n", failures, &"s"[failures == 1]);
  else
    puts ("Test succeeded.");

  return failures;
}

#include <support/test-driver.c>
