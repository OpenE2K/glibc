/* e2k is currently lacking sfp-machine.h header required by this
   implementation.  */
/* #include <sysdeps/x86/fpu/e_sqrtf128.c>  */

/* This fake implementation just lets one formally make
   sysdeps/ieee754/float128 for e2k at present.  */
__float128
__ieee754_sqrtf128 (__float128 a)
{
  return a;
}
