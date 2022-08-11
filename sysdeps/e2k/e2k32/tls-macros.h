#define TLS_LE(x)                                                       \
  ({ int *__l;                                                          \
    asm ("{\n\t"                                                        \
         "adds %%g13, [ $"#x"@TLS_LE ], %0"                             \
         "}\n\t"                                                        \
         : "=r" (__l));                                                 \
    __l; })

#define TLS_LOAD_PIC                                             \
  ({ long pc, got;                                               \
    asm ("{\n\t"                                                 \
         "rrd,0 %%ip, %0\n\t"                                    \
         "addd,1 0, [ _lts1 $_GLOBAL_OFFSET_TABLE_ ], %1\n\t"      \
         "}\n\t"                                                 \
         "addd %1, %0, %1\n\t"                                   \
         : "=r" (pc), "=r" (got));                               \
    got; })


#define TLS_IE(x)                                 \
  ({ int *__l;                                    \
    asm ("{\n\t"                                  \
         "ldgdw %1, [ $"#x"@TLS_IE ], %0\n\t"     \
         "}\n\t"                                   \
         "adds %%g13, %0, %0"                     \
         : "=r" (__l) : "r" (TLS_LOAD_PIC));      \
    __l; })
  

#define TLS_LD(x)                                          \
  ({                                                       \
    long __m, __o;                                         \
    int *__l;                                              \
    asm ("{\n\t"                                           \
         "disp %%ctpr1, __tls_get_addr\n\t"                \
         "ldgdw %3, [ $"#x"@TLS_GDMOD], %0\n\t"            \
         "ldgdw %3, [ $"#x"@TLS_GDREL], %1\n\t"            \
         "}\n\t"                                           \
         "{\n\t"                                           \
         "adds 0, %0, %%b[0]\n\t"                          \
         "adds 0, %1, %%b[1]\n\t"                          \
         "call %%ctpr1, wbs=%#\n\t"                        \
         "}\n\t"                                           \
         "adds 0, %%db[0], %2\n\t"                         \
         : "=r" (__m), "=r" (__o), "=r" (__l)              \
         : "r" (TLS_LOAD_PIC)                              \
         : "call", "memory");                              \
    __l; })



#define TLS_GD(x)                                          \
  ({                                                       \
    long __m, __o;                                         \
    int *__l;                                              \
    asm ("{\n\t"                                           \
         "disp %%ctpr1, __tls_get_addr\n\t"                \
         "ldgdw %3, [ $"#x"@TLS_GDMOD], %0\n\t"            \
         "ldgdw %3, [ $"#x"@TLS_GDREL], %1\n\t"            \
         "}\n\t"                                           \
         "{\n\t"                                           \
         "adds 0, %0, %%b[0]\n\t"                          \
         "adds 0, %1, %%b[1]\n\t"                          \
         "call %%ctpr1, wbs=%#\n\t"                        \
         "}\n\t"                                           \
         "adds 0, %%db[0], %2\n\t"                         \
         : "=r" (__m), "=r" (__o), "=r" (__l)              \
         : "r" (TLS_LOAD_PIC)                              \
         : "call", "memory");                              \
    __l; })
