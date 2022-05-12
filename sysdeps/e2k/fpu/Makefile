ifeq ($(subdir),math)
# See Bug #105130, Comment #2 for why fraiseexcpt.c should be compiled with
# -O0 on E2K. Or with -fno-spec-fp option
CFLAGS-fraiseexcpt.c += -fno-spec-fp

# See Bug #106328, Comment #4 for why s_fma/f/l.c should be compiled with
# -O0 on E2K. Or with -fno-spec-fp option
CFLAGS-s_fmaf.c += -fno-spec-fp
CFLAGS-s_fma.c += -fno-spec-fp
CFLAGS-s_fmal.c += -fno-spec-fp

# See Bug #118896
CFLAGS-k_standardl.c += -fno-spec-fp
CFLAGS-e_expf128.c += -fno-spec-fp
CFLAGS-s_fadd.c += -fno-spec-fp
CFLAGS-s_faddl.c += -fno-spec-fp
CFLAGS-s_daddl.c += -fno-spec-fp
CFLAGS-s_f32addf128.c += -fno-spec-fp
CFLAGS-s_f64addf128.c += -fno-spec-fp
CFLAGS-s_f64xaddf128.c += -fno-spec-fp
CFLAGS-s_fdiv.c += -fno-spec-fp
CFLAGS-s_fdivl.c += -fno-spec-fp
CFLAGS-s_ddivl.c += -fno-spec-fp
CFLAGS-s_f32divf128.c += -fno-spec-fp
CFLAGS-s_f64divf128.c += -fno-spec-fp
CFLAGS-s_f64xdivf128.c += -fno-spec-fp
CFLAGS-s_fmul.c += -fno-spec-fp
CFLAGS-s_fmull.c += -fno-spec-fp
CFLAGS-s_dmull.c += -fno-spec-fp
CFLAGS-s_f32mulf128.c += -fno-spec-fp
CFLAGS-s_f64mulf128.c += -fno-spec-fp
CFLAGS-s_f64xmulf128.c += -fno-spec-fp
CFLAGS-s_fsub.c += -fno-spec-fp
CFLAGS-s_fsubl.c += -fno-spec-fp
CFLAGS-s_dsubl.c += -fno-spec-fp
CFLAGS-s_f32subf128.c += -fno-spec-fp
CFLAGS-s_f64subf128.c += -fno-spec-fp
CFLAGS-s_f64xsubf128.c += -fno-spec-fp

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
