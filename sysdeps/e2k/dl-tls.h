/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Thread-local storage handling in the ELF dynamic linker.  E2K version.
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

#ifndef tls_e2k_m_offset_type
# define tls_e2k_m_offset_type size_t
#endif /* tls_e2k_m_offset_type  */


/* On E2K the __tls_get_addr function take the module ID and the
   offset as parameters.  */
#define GET_ADDR_ARGS		tls_e2k_m_offset_type tls_e2k_m, tls_e2k_m_offset_type tls_e2k_offset
#define GET_ADDR_PARAM		tls_e2k_m, tls_e2k_offset
#define GET_ADDR_MODULE		tls_e2k_m
#define GET_ADDR_OFFSET		tls_e2k_offset

/* We have no tls_index type.  */
#define DONT_USE_TLS_INDEX	1

extern void *__tls_get_addr (tls_e2k_m_offset_type m,
			     tls_e2k_m_offset_type offset);

/* Value used for dtv entries for which the allocation is delayed.  */
#define TLS_DTV_UNALLOCATED	((void *) -1l)
