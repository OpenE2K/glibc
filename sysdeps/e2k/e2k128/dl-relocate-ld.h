/* Check if dynamic section should be relocated.  E2K128 version.
   Copyright (C) 2021 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.  */

#ifndef _DL_RELOCATE_LD_H
#define _DL_RELOCATE_LD_H

/* Prevent `d_ptr's in `.dynamic' section from being adjusted in a pointless
   way from the point of view of Protected Mode despite the fact that this
   section is typically writable in ELF (for no good reason). TODO: the
   latter should probably be changed one day.  */
#define DL_RO_DYN_SECTION 1

#endif /* _DL_RELOCATE_LD_H */
