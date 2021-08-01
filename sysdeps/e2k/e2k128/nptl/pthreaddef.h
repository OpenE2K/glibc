/* Default stack size.  */
#define ARCH_STACK_DEFAULT_SIZE	(4 * 1024 * 1024)

/* Required stack pointer alignment at beginning.  */
#define STACK_ALIGN		16

/* Minimal stack size after allocating thread descriptor and guard size.  */
#define MINIMAL_REST_STACK	4096

/* Alignment requirement for TCB.  */
#define TCB_ALIGNMENT		16

/* Location of current stack frame.  */
#define CURRENT_STACK_FRAME __builtin_frame_address (0)
