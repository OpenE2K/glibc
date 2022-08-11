/*
  This is a version (aka dlmalloc) of malloc/free/realloc written by
  Doug Lea and released to the public domain.  Use, modify, and
  redistribute this code without permission or acknowledgement in any
  way you wish.  Send questions, comments, complaints, performance
  data, etc to dl@cs.oswego.edu

  VERSION 2.7.2 Sat Aug 17 09:07:30 2002  Doug Lea  (dl at gee)

  Note: There may be an updated version of this malloc obtainable at
           ftp://gee.cs.oswego.edu/pub/misc/malloc.c
  Check before installing!

  Hacked up for uClibc by Erik Andersen <andersen@codepoet.org>
*/


/* ------------------------------ realloc ------------------------------ */
void* realloc(void* oldmem, size_t bytes)
{
    mstate av;

    size_t  nb;              /* padded request size */

    mchunkptr        oldp;            /* chunk corresponding to oldmem */
    size_t  oldsize;         /* its size */

    mchunkptr        newp;            /* chunk to return */
    size_t  newsize;         /* its size */
    void*          newmem;          /* corresponding user mem */

    mchunkptr        next;            /* next contiguous chunk after oldp */

    mchunkptr        remainder;       /* extra space at end of newp */
    unsigned long     remainder_size;  /* its size */

    mchunkptr        bck;             /* misc temp for linking */
    mchunkptr        fwd;             /* misc temp for linking */

    unsigned long     copysize;        /* bytes to copy */
#ifndef __PROTECTED__ /* Unused variable in PM */
    unsigned int     ncopies;         /* size_t words to copy */
#endif
    size_t* s;               /* copy source */
    size_t* d;               /* copy destination */

    void *retval;

    /* Check for special cases.  */
    if (! oldmem)
	return malloc(bytes);
    if (! bytes) {
	free (oldmem);
	return NULL;
    }

    checked_request2size(bytes, nb);
    __MALLOC_LOCK;
    av = get_malloc_state();

#ifdef __PROTECTED__
    void* user_oldmem = oldmem;

    oldmem = pmem2mem(oldmem, av);
#endif
    oldp    = mem2chunk(oldmem);
    oldsize = chunksize(oldp);

    check_inuse_chunk(oldp);

    if (!chunk_is_mmapped(oldp)) {

	if ((unsigned long)(oldsize) >= (unsigned long)(nb)) {
#ifndef __PROTECTED__
	    /* already big enough; split below */
	    newp = oldp;
	    newsize = oldsize;
#else /* __PROTECTED__ */
	    /* For PM we have 2 ways here: malloc-copy-free or
	       return to user previous big descriptor */
	    /* Return previous big descriptor */
	    retval = oldmem;	/* oldmem, not oldchunk */
	    bytes = oldsize - (sizeof(size_t)); /* chunksize except size of
						   allocated chunk's head */
	    goto DONE;
#endif
	}

	else {
	    next = chunk_at_offset(oldp, oldsize);

	    /* Try to expand forward into top */
	    if (next == av->top &&
		    (unsigned long)(newsize = oldsize + chunksize(next)) >=
		    (unsigned long)(nb + MINSIZE)) {
		set_head_size(oldp, nb);
		av->top = chunk_at_offset(oldp, nb);
		set_head(av->top, (newsize - nb) | PREV_INUSE);
		retval = chunk2mem(oldp);
		goto DONE;
	    }

	    /* Try to expand forward into next chunk;  split off remainder below */
	    else if (next != av->top &&
		    !inuse(next) &&
		    (unsigned long)(newsize = oldsize + chunksize(next)) >=
		    (unsigned long)(nb)) {
		newp = oldp;
		unlink(next, bck, fwd);
	    }

	    /* allocate, copy, free */
	    else {
#ifndef __PROTECTED__
		newmem = malloc(nb - MALLOC_ALIGN_MASK);
#else /* __PROTECTED__ */
		newmem = malloc_internal_1 (nb - MALLOC_ALIGN_MASK, 0);
#endif /* __PROTECTED__ */
		if (newmem == 0) {
		    retval = 0; /* propagate failure */
		    goto DONE;
		}

		newp = mem2chunk(newmem);
		newsize = chunksize(newp);

		/*
		   Avoid copy if newp is next chunk after oldp.
		   */
		if (newp == next) {
		    newsize += oldsize;
		    newp = oldp;
		}
		else {
#ifndef __PROTECTED__
		    /*
		       Unroll copy of <= 36 bytes (72 if 8byte sizes)
		       We know that contents have an odd number of
		       size_t-sized words; minimally 3.
		       */

		    copysize = oldsize - (sizeof(size_t));
		    s = (size_t*)(oldmem);
		    d = (size_t*)(newmem);
		    ncopies = copysize / sizeof(size_t);
		    assert(ncopies >= 3);

		    if (ncopies > 9)
			memcpy(d, s, copysize);

		    else {
			*(d+0) = *(s+0);
			*(d+1) = *(s+1);
			*(d+2) = *(s+2);
			if (ncopies > 4) {
			    *(d+3) = *(s+3);
			    *(d+4) = *(s+4);
			    if (ncopies > 6) {
				*(d+5) = *(s+5);
				*(d+6) = *(s+6);
				if (ncopies > 8) {
				    *(d+7) = *(s+7);
				    *(d+8) = *(s+8);
				}
			    }
			}
		    }
#else /* __PROTECTED__ e2k */
		    /* Do not want to do tricks, just copy with memcpy */
		    /* Copy whole chunk except 'size of next chunk' field.
		       As for 64 bit mode */
		    copysize = oldsize - (sizeof(size_t));
		    s = (size_t*)(oldmem);
                    d = (size_t*)(newmem);
		    memcpy(d, s, copysize);
#endif /* __PROTECTED__ e2k */

#ifndef __PROTECTED__
		    free(oldmem);
#else /* __PROTECTED__ */
		    /* Call free here, not free_internal, because user_oldmem
		    descriptor copies should be cleaned from user memory */
		    free_1(user_oldmem, 0);
#endif /* __PROTECTED__ */
		    check_inuse_chunk(newp);
		    retval = chunk2mem(newp);
		    goto DONE;
		}
	    }
	}

	/* If possible, free extra space in old or extended chunk */

	assert((unsigned long)(newsize) >= (unsigned long)(nb));

	remainder_size = newsize - nb;

	if (remainder_size < MINSIZE) { /* not enough extra to split off */
	    set_head_size(newp, newsize);
	    set_inuse_bit_at_offset(newp, newsize);
	}
	else { /* split remainder */
	    remainder = chunk_at_offset(newp, nb);
	    set_head_size(newp, nb);
	    set_head(remainder, remainder_size | PREV_INUSE);
	    /* Mark remainder as inuse so free() won't complain */
	    set_inuse_bit_at_offset(remainder, remainder_size);
#ifndef __PROTECTED__
	    free(chunk2mem(remainder));
#else /* __PROTECTED__ */
	    /* Do not need to clean user memory because remainder was not held
	       by user */
	    free_internal(chunk2mem(remainder));
#endif /* __PROTECTED__ */
	}

	check_inuse_chunk(newp);
	retval = chunk2mem(newp);
	goto DONE;
    }

    /*
       Handle mmap cases
       */

    else {
	size_t offset = oldp->prev_size;
	size_t pagemask = av->pagesize - 1;
#if 0
	/* These variables are used in the currently unworkable mremap ()
	   branch only.  */
	char *cp;
	unsigned long  sum;
#endif /* 0  */

	/* Note the extra (sizeof(size_t)) overhead */
	newsize = (nb + offset + (sizeof(size_t)) + pagemask) & ~pagemask;

	/* don't need to remap if still within same page */
	if (oldsize == newsize - offset) {
	    retval = oldmem;
	    goto DONE;
	}

#ifdef __PROTECTED__
	/* FIXME: I suspect that this call of `delete_region ()' here makes
	   sense only if `mremap ()' two lines below succeeds. If it fails,
	   which is sure to be the case with up-to-date PM glibc in use,
	   `delete_region ()' will probably be called once again this time
	   on behalf of `free_1 ()'. This seems to be the reason for a failure
	   in Bug #106901 after commenting out unworkable `mremap ()' branch
	   and thus migrating to `malloc/free ()' implementation.  */
#if 0
	delete_region(oldp, av);
#endif /* 0  */
#endif /* __PROTECTED__ */

#if 0
	/* FIXME: stupidly inhibit unworkable `mremap ()' branch for now because
	   PM `mremap ()' seems to be incapable of returning AP despite the fact
	   that the syscall seems to succeed. The returned value is probably a
	   virtual address.  */
	cp = (char*)mremap((char*)oldp - offset, oldsize + offset, newsize, 1);

	if (cp != (char*)MORECORE_FAILURE) {

#ifdef __PROTECTED__
	    /* Add new region */
	    ((mprotptr)(cp + offset - sizeof(mprotptr)))->fd =
							     av->mmaped_region;
	    av->mmaped_region = (mprotptr)cp;
	    cp += sizeof(mprotptr);
#endif /* __PROTECTED__ */

	    newp = (mchunkptr)(cp + offset);
	    set_head(newp, (newsize - offset)|IS_MMAPPED);

	    assert(aligned_OK(chunk2mem(newp)));
	    assert((newp->prev_size == offset));

	    /* update statistics */
	    sum = av->mmapped_mem += newsize - oldsize;
	    if (sum > (unsigned long)(av->max_mmapped_mem))
		av->max_mmapped_mem = sum;
	    sum += av->sbrked_mem;
	    if (sum > (unsigned long)(av->max_total_mem))
		av->max_total_mem = sum;

	    retval = chunk2mem(newp);
	    goto DONE;
	}
#endif /* 0  */

	/* Note the extra (sizeof(size_t)) overhead. */
	if ((unsigned long)(oldsize) >= (unsigned long)(nb + (sizeof(size_t))))
	    newmem = oldmem; /* do nothing */
	else {
	    /* Must alloc, copy, free. */
#ifndef __PROTECTED__
	    newmem = malloc(nb - MALLOC_ALIGN_MASK);
#else /* __PROTECTED__ */
	    newmem = malloc_internal_1 (nb - MALLOC_ALIGN_MASK, 0);
#endif /* __PROTECTED__ */
	    if (newmem != 0) {
		memcpy(newmem, oldmem, oldsize - 2*(sizeof(size_t)));
#ifndef __PROTECTED__
		free(oldmem);
#else /* __PROTECTED__  */
		free_1(oldmem, 0);
#endif /* __PROTECTED__  */
	    }
	}
	retval = newmem;
    }

 DONE:
    __MALLOC_UNLOCK;
#ifdef __PROTECTED__
    retval = mem2pmem(retval, bytes);
#endif
    return retval;
}

