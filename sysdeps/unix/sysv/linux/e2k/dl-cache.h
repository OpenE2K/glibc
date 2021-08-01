/* Support for reading /etc/ld.so.cache files written by Linux ldconfig.
   Copyright (C) 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#define add_system_dir(dir)						\
  do									\
    {									\
      size_t len = strlen (dir);					\
      /* An extra char may be required because of a possible		\
	 "{32,64} --> 128" replacement in DIR.  */			\
      char path[len + 2];						\
      memcpy (path, dir, len + 1);					\
									\
      if ((len >= 6 && ! memcmp (path + len - 6, "/lib32", 6)))		\
	{								\
	  /* 32 --> 64  */						\
	  memcpy (path + len - 2, "64", 3);				\
	}								\
      else if (len >= 7 && ! memcmp (path + len - 7, "/lib128", 7))	\
	{								\
	  /* 128 --> 64 reduces LEN by 1.  */				\
	  memcpy (path + len - 3, "64", 3);				\
	  len--;							\
	}								\
									\
      /* Add DIR either in its original form  or with {32,128}		\
	 replaced by 64.  */						\
      add_dir (path);							\
									\
      if (len >= 6 && ! memcmp (path + len - 6, "/lib64", 6))		\
	{								\
	  /* 64 --> 32  */						\
	  memcpy (path + len - 2, "32", 3);				\
	  add_dir (path);						\
	}								\
									\
      if (len >= 6 && ! memcmp (path + len - 6, "/lib32", 6))		\
	{								\
	  /* 32 --> 128 increases LEN by 1, but this does not matter as	\
	     no replacements are to be done anymore.  */		\
	  memcpy (path + len - 2, "128", 4);				\
	  add_dir (path);						\
	}								\
									\
    } while (0)

#include <sysdeps/generic/dl-cache.h>
