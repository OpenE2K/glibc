/* Copyright (C) 2004-2019 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _BITS_STRING_FORTIFIED_H
#define _BITS_STRING_FORTIFIED_H 1

#ifndef _STRING_H
# error "Never use <bits/string_fortified.h> directly; include <string.h> instead."
#endif

#if !__GNUC_PREREQ (5,0)
__warndecl (__warn_memset_zero_len,
	    "memset used with constant zero length parameter; this could be due to transposed parameters");
#endif

__fortify_function void *
__NTH (memcpy (void *__restrict __dest, const void *__restrict __src,
	       size_t __len))
{
  return __builtin___memcpy_chk (__dest, __src, __len, __bos0 (__dest));
}

__fortify_function void *
__NTH (memmove (void *__dest, const void *__src, size_t __len))
{
  return __builtin___memmove_chk (__dest, __src, __len, __bos0 (__dest));
}

#ifdef __USE_GNU
__fortify_function void *
__NTH (mempcpy (void *__restrict __dest, const void *__restrict __src,
		size_t __len))
{
  return __builtin___mempcpy_chk (__dest, __src, __len, __bos0 (__dest));
}
#endif


/* The first two tests here help to catch a somewhat common problem
   where the second and third parameter are transposed.  This is
   especially problematic if the intended fill value is zero.  In this
   case no work is done at all.  We detect these problems by referring
   non-existing functions.  */
__fortify_function void *
__NTH (memset (void *__dest, int __ch, size_t __len))
{
  /* GCC-5.0 and newer implements these checks in the compiler, so we don't
     need them here.  */
#if !__GNUC_PREREQ (5,0)
  if (__builtin_constant_p (__len) && __len == 0
      && (!__builtin_constant_p (__ch) || __ch != 0))
    {
      __warn_memset_zero_len ();
      return __dest;
    }
#endif
  return __builtin___memset_chk (__dest, __ch, __len, __bos0 (__dest));
}

#ifdef __USE_MISC
# include <bits/strings_fortified.h>

void __explicit_bzero_chk (void *__dest, size_t __len, size_t __destlen)
  __THROW __nonnull ((1));

__fortify_function void
__NTH (explicit_bzero (void *__dest, size_t __len))
{
  __explicit_bzero_chk (__dest, __len, __bos0 (__dest));
}
#endif

__fortify_function char *
__NTH (strcpy (char *__restrict __dest, const char *__restrict __src))
{
  return __builtin___strcpy_chk (__dest, __src, __bos (__dest));
}

#ifdef __USE_GNU
__fortify_function char *
__NTH (stpcpy (char *__restrict __dest, const char *__restrict __src))
{
  return __builtin___stpcpy_chk (__dest, __src, __bos (__dest));
}
#endif


__fortify_function char *
__NTH (strncpy (char *__restrict __dest, const char *__restrict __src,
		size_t __len))
{
  return __builtin___strncpy_chk (__dest, __src, __len, __bos (__dest));
}

/* XXX We have no corresponding builtin yet.  */
extern char *__stpncpy_chk (char *__dest, const char *__src, size_t __n,
			    size_t __destlen) __THROW;
extern char *__REDIRECT_NTH (__stpncpy_alias, (char *__dest, const char *__src,
					       size_t __n), stpncpy);

__fortify_function char *
__NTH (stpncpy (char *__dest, const char *__src, size_t __n))
{
  if (__bos (__dest) != (size_t) -1
      && (!__builtin_constant_p (__n) || __n > __bos (__dest)))
    return __stpncpy_chk (__dest, __src, __n, __bos (__dest));
  return __stpncpy_alias (__dest, __src, __n);
}


__fortify_function char *
__NTH (strcat (char *__restrict __dest, const char *__restrict __src))
{
  return __builtin___strcat_chk (__dest, __src, __bos (__dest));
}


__fortify_function char *
__NTH (strncat (char *__restrict __dest, const char *__restrict __src,
		size_t __len))
{
  return __builtin___strncat_chk (__dest, __src, __len, __bos (__dest));
}

#ifdef __USE_MISC
__warndecl (__warn_strlcpy_size_zero,
	    "strlcpy used with a size argument of zero");
__warndecl (__warn_strlcpy_size_large,
	    "strlcpy used with a size argument which is too large");
extern size_t __strlcpy_chk (char *__dest, const char *__src, size_t __n,
			     size_t __destlen) __THROW;
extern size_t __REDIRECT_NTH (__strlcpy_alias,
			      (char *__dest, const char *__src, size_t __n),
			      strlcpy);

__fortify_function size_t
__NTH (strlcpy (char *__restrict __dest, const char *__restrict __src,
		size_t __len))
{
  if (__builtin_constant_p (__len == 0) && __len == 0)
    {
      __warn_strlcpy_size_zero ();
      return 0;
    }
  if (__builtin_constant_p (__len > __bos (__dest)) && __len > __bos (__dest))
    __warn_strlcpy_size_large ();
  if (__builtin_constant_p (__bos (__dest) == (size_t) -1)
      && __bos (__dest) == (size_t) -1)
    return __strlcpy_alias (__dest, __src, __len);
  return __strlcpy_chk (__dest, __src, __len, __bos (__dest));
}

__warndecl (__warn_strlcat_size_zero,
	    "strlcat used with a size argument of zero");
__warndecl (__warn_strlcat_size_large,
	    "strlcat used with a size argument which is too large");
extern size_t __strlcat_chk (char *__dest, const char *__src, size_t __n,
			     size_t __destlen) __THROW;
extern size_t __REDIRECT_NTH (__strlcat_alias,
			      (char *__dest, const char *__src, size_t __n),
			      strlcat);

__fortify_function size_t
__NTH (strlcat (char *__restrict __dest, const char *__restrict __src,
		size_t __len))
{
  if (__builtin_constant_p (__len == 0) && __len == 0)
    {
      __warn_strlcat_size_zero ();
      return strlen (__src);
    }
  if (__builtin_constant_p (__len > __bos (__dest)) && __len > __bos (__dest))
    __warn_strlcat_size_large ();
  if (__builtin_constant_p (__bos (__dest) == (size_t) -1)
      && __bos (__dest) == (size_t) -1)
    return __strlcat_alias (__dest, __src, __len);
  return __strlcat_chk (__dest, __src, __len, __bos (__dest));
}
#endif /* __USE_MISC */

#endif /* bits/string_fortified.h */
