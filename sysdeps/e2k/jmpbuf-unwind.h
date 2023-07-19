/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#if ! defined __LCC_SJLJ_EXCEPTIONS__
/* Definitions of E2K-specific register types and macros.  */

#define	E2K_VA_SIZE	48
#define	E2K_VA_MSB	(E2K_VA_SIZE - 1)

typedef	struct e2k_rwap_lo_fields {	/* Fields of lower word */
  unsigned long long base : E2K_VA_SIZE;	/* [47: 0] */
  unsigned long long unused2 : 55 - E2K_VA_MSB; /* [55:48] */
  unsigned long long stub3 : 1;			/* [56] */
  unsigned long long stub2 : 1;			/* [57] */
  unsigned long long stub1 : 1;			/* [58] */
  unsigned long long rw : 2;			/* [60:59] */
  unsigned long long itag : 3;			/* [63:61] */
} e2k_rwap_lo_fields_t;


/* Structure of lower word */
typedef	union e2k_rwap_lo_struct {
  e2k_rwap_lo_fields_t fields;		/* as AP fields */
  unsigned long long word;		/* as entire register */
} e2k_rwap_lo_struct_t;

/* Fields of high word */
typedef	struct e2k_rwap_hi_fields {
unsigned long long curptr : 32;			/* [31: 0] */
unsigned long long size : 32;			/* [63:32] */
} e2k_rwap_hi_fields_t;

/* Structure of high word */
typedef	union e2k_rwap_hi_struct {
  e2k_rwap_hi_fields_t	fields;	/* as AP fields */
  unsigned long long word;	/* as entire register */
} e2k_rwap_hi_struct_t;

typedef e2k_rwap_lo_struct_t e2k_pcsp_lo_t;
typedef e2k_rwap_lo_struct_t e2k_psp_lo_t;
typedef e2k_rwap_hi_struct_t e2k_pcsp_hi_t;
typedef e2k_rwap_hi_struct_t e2k_psp_hi_t;

#define E2K_PCSHTP_SIZE 11
#define PCSHTP_SIGN_EXTEND(pcshtp)					\
  ((unsigned long long) (((long long) (pcshtp) << (64 - E2K_PCSHTP_SIZE)) \
			 >> (64 - E2K_PCSHTP_SIZE)))

static inline int
__jmpbuf_cfa_unwinds_adj (__jmp_buf jmpbuf,
                          struct _Unwind_Context *context,
                          uintptr_t adj)
{
#if 0
  /* Not currently used for anything.  */
  e2k_pcsp_lo_t pcsp_lo;
#endif /* 0  */
  e2k_pcsp_hi_t pcsp_hi;
  unsigned int pcshtp;

#if 0
  /* See above. Just left as a reminder on where %pcsp_lo may be fetched
     from if needed.  */
  pcsp_lo.word = jmpbuf[3];
#endif
  pcsp_hi.word = jmpbuf[4];
  pcshtp = *((unsigned int *) &jmpbuf[5]);

  /* '- 0x20' accounts for the fact that %PCSP stored in JMPBUF is read within
     the context of 'setjmp ()', not within the function from which 'setjmp ()'
     is invoked.  */
  return ((unsigned long long) _Unwind_GetPCSP (context)
          > (pcsp_hi.fields.curptr + PCSHTP_SIGN_EXTEND (pcshtp) - 0x20));
}

#define _JMPBUF_CFA_UNWINDS_ADJ(_jmpbuf, _context, _adj) \
  __jmpbuf_cfa_unwinds_adj (_jmpbuf, _context, _adj)

#endif /* ! defined __LCC_SJLJ_EXCEPTIONS__  */

#define _JMPBUF_UNWINDS(jmpbuf, address, demangle) (*((int*)0x0) = 0x50)
#define _JMPBUF_UNWINDS_ADJ(_jmpbuf, _address, _adj) (*((int*)0x0) = 0x52, 1)


#define __libc_unwind_longjmp(buf, val) __libc_longjmp (buf, val)
