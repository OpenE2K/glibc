#if !HAVE_TUNABLES
# define GLIBC_TUNABLES_ENVVAR "GLIBC_TUNABLES\0"
#else
# define GLIBC_TUNABLES_ENVVAR
#endif

/* Environment variable to be removed for SUID programs.  The names are
   all stuffed in a single string which means they have to be terminated
   with a '\0' explicitly.  */
#define UNSECURE_ENVVARS \
  "ARGP_HELP_FMT\0"							\
  "DATEMSK\0"								\
  "GCONV_PATH\0"							\
  "GETCONF_DIR\0"							\
  "GLIBC_TUNABLES\0"							\
  "GMON_OUT_PREFIX\0"							\
  "HESIOD_CONFIG\0"							\
  "HES_DOMAIN\0"							\
  "HOSTALIASES\0"							\
  "LD_AUDIT\0"								\
  "LD_BIND_NOT\0"							\
  "LD_BIND_NOW\0"							\
  "LD_DEBUG\0"								\
  "LD_DEBUG_OUTPUT\0"							\
  "LD_DYNAMIC_WEAK\0"							\
  "LD_HWCAP_MASK\0"							\
  "LD_LIBRARY_PATH\0"							\
  "LD_ORIGIN_PATH\0"							\
  "LD_PRELOAD\0"							\
  "LD_PROFILE\0"							\
  "LD_PROFILE_OUTPUT\0"							\
  "LD_SHOW_AUXV\0"							\
  "LD_TRACE_LOADED_OBJECTS\0"						\
  "LD_TRACE_PRELINKING\0"						\
  "LD_USE_LOAD_BIAS\0"							\
  "LD_VERBOSE\0"							\
  "LD_WARN\0"								\
  "LOCALDOMAIN\0"							\
  "LOCPATH\0"								\
  "MALLOC_CHECK_\0"							\
  "MALLOC_MMAP_MAX_\0"							\
  "MALLOC_MMAP_THRESHOLD_\0"						\
  "MALLOC_PERTURB_\0"							\
  "MALLOC_TOP_PAD_\0"							\
  "MALLOC_TRACE\0"							\
  "MALLOC_TRIM_THRESHOLD_\0"						\
  "MEMUSAGE_BUFFER_SIZE\0"						\
  "MEMUSAGE_NO_TIMER\0"							\
  "MEMUSAGE_OUTPUT\0"							\
  "MEMUSAGE_PROG_NAME\0"						\
  "MEMUSAGE_TRACE_MMAP\0"						\
  "MSGVERB\0"								\
  "NIS_DEFAULTS\0"							\
  "NIS_GROUP\0"								\
  "NIS_PATH\0"								\
  "NLSPATH\0"								\
  "PCPROFILE_OUTPUT\0"							\
  "POSIXLY_CORRECT\0"							\
  "PWD\0"								\
  "RESOLV_ADD_TRIM_DOMAINS\0"						\
  "RESOLV_HOST_CONF\0"							\
  "RESOLV_MULTI\0"							\
  "RESOLV_OVERRIDE_TRIM_DOMAINS\0"					\
  "RESOLV_REORDER\0"							\
  "RESOLV_SPOOF_CHECK\0"						\
  "RES_OPTIONS\0"							\
  "SEGFAULT_OUTPUT_NAME\0"						\
  "SEGFAULT_SIGNALS\0"							\
  "SEGFAULT_USE_ALTSTACK\0"						\
  "SEV_LEVEL\0"								\
  "TZ\0"								\
  "TZDIR\0"

#define UNSECURE_UID_ENVVARS \
  "TMPDIR\0"

#define RESTRICTED_ENVVARS \
  "LANG\0"								\
  "LANGUAGE\0"								\
  "LC_ADDRESS\0"							\
  "LC_ALL\0"								\
  "LC_COLLATE\0"							\
  "LC_CTYPE\0"								\
  "LC_IDENTIFICATION\0"							\
  "LC_MEASUREMENT\0"							\
  "LC_MESSAGES\0"							\
  "LC_MONETARY\0"							\
  "LC_NAME\0"								\
  "LC_NUMERIC\0"							\
  "LC_PAPER\0"								\
  "LC_TELEPHONE\0"							\
  "LC_TIME\0"								\
  "LC_XXX\0"
