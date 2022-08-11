ifeq ($(subdir),math)
# See Bug #105130, Comment #2 for why fraiseexcpt.c should be compiled with
# -O0 on E2K. Or with -fno-fp-spec option
CFLAGS-fraiseexcpt.c += -fno-fp-spec

# See Bug #106328, Comment #4 for why s_fma/f/l.c should be compiled with
# -O0 on E2K. Or with -fno-fp-spec option
CFLAGS-s_fmaf.c += -fno-fp-spec
CFLAGS-s_fma.c += -fno-fp-spec
CFLAGS-s_fmal.c += -fno-fp-spec

# See Bug #118896
CFLAGS-k_standardl.c += -fno-fp-spec
CFLAGS-e_expf128.c += -fno-fp-spec
CFLAGS-s_fadd.c += -fno-fp-spec
CFLAGS-s_faddl.c += -fno-fp-spec
CFLAGS-s_daddl.c += -fno-fp-spec
CFLAGS-s_f32addf128.c += -fno-fp-spec
CFLAGS-s_f64addf128.c += -fno-fp-spec
CFLAGS-s_f64xaddf128.c += -fno-fp-spec
CFLAGS-s_fdiv.c += -fno-fp-spec
CFLAGS-s_fdivl.c += -fno-fp-spec
CFLAGS-s_ddivl.c += -fno-fp-spec
CFLAGS-s_f32divf128.c += -fno-fp-spec
CFLAGS-s_f64divf128.c += -fno-fp-spec
CFLAGS-s_f64xdivf128.c += -fno-fp-spec
CFLAGS-s_fmul.c += -fno-fp-spec
CFLAGS-s_fmull.c += -fno-fp-spec
CFLAGS-s_dmull.c += -fno-fp-spec
CFLAGS-s_f32mulf128.c += -fno-fp-spec
CFLAGS-s_f64mulf128.c += -fno-fp-spec
CFLAGS-s_f64xmulf128.c += -fno-fp-spec
CFLAGS-s_fsub.c += -fno-fp-spec
CFLAGS-s_fsubl.c += -fno-fp-spec
CFLAGS-s_dsubl.c += -fno-fp-spec
CFLAGS-s_f32subf128.c += -fno-fp-spec
CFLAGS-s_f64subf128.c += -fno-fp-spec
CFLAGS-s_f64xsubf128.c += -fno-fp-spec

# See Bug #109049, #109054
CFLAGS-e_hypot.c += -fno-rounding-math

CFLAGS-s_ccos.c += -fno-rounding-math
CFLAGS-s_ccosh.c += -fno-rounding-math
CFLAGS-s_cexp.c += -fno-rounding-math
CFLAGS-s_csin.c += -fno-rounding-math
CFLAGS-s_csinh.c += -fno-rounding-math
CFLAGS-s_csqrt.c += -fno-rounding-math

CFLAGS-s_cos.c += -fno-rounding-math
CFLAGS-s_sin.c += -fno-rounding-math
CFLAGS-s_sincos.c += -fno-rounding-math
CFLAGS-s_tan.c += -fno-rounding-math
endif
