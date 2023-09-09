/* Useful if you wish to make target-specific GCC changes. */
#undef TARGET_SYX
#define TARGET_SYX 1

/* Default arguments you want when running your
   i686-myos-gcc/x86_64-myos-gcc toolchain */
#undef LIB_SPEC
#define LIB_SPEC "-lc" /* link against C standard library */

   /* Files that are linked before user code.
      The %s tells GCC to look for these files in the library directory. */
#undef STARTFILE_SPEC
//#define STARTFILE_SPEC "crt0.o%s crti.o%s crtbegin.o%s"
#define STARTFILE_SPEC "%{!shared: %{!symbolic: %{!p:Scrt1.o%s}}} crtbegin.o%s"

      /* Files that are linked after user code. */
#undef ENDFILE_SPEC
//#define ENDFILE_SPEC "crtend.o%s crtn.o%s"
#define ENDFILE_SPEC "crtend.o%s"

#undef LINK_SPEC
#define LINK_SPEC "%{shared:-shared} %{static:-static} %{!shared: %{!static: %{rdynamic:-export-dynamic}}}"

/* Additional predefined macros. */
#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()      \
  do {                                \
    builtin_define ("__syx__");      \
    builtin_define ("__unix__");      \
    builtin_assert ("system=syx");   \
    builtin_assert ("system=unix");   \
    builtin_assert ("system=posix");   \
  } while(0);

/* musl avoids problematic includes by rearranging the include directories.
 * Unfortunately, this is mostly duplicated from cppdefault.c */

#define INCLUDE_DEFAULTS_MUSL_GPP			\
    { GPLUSPLUS_INCLUDE_DIR, "G++", 1, 1,		\
      GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 0 },		\
    { GPLUSPLUS_TOOL_INCLUDE_DIR, "G++", 1, 1,		\
      GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 1 },		\
    { GPLUSPLUS_BACKWARD_INCLUDE_DIR, "G++", 1, 1,	\
      GPLUSPLUS_INCLUDE_DIR_ADD_SYSROOT, 0 },

#ifdef LOCAL_INCLUDE_DIR
#define INCLUDE_DEFAULTS_MUSL_LOCAL			\
    { LOCAL_INCLUDE_DIR, 0, 0, 1, 1, 2 },		\
    { LOCAL_INCLUDE_DIR, 0, 0, 1, 1, 0 },
#else
#define INCLUDE_DEFAULTS_MUSL_LOCAL
#endif

#ifdef PREFIX_INCLUDE_DIR
#define INCLUDE_DEFAULTS_MUSL_PREFIX			\
    { PREFIX_INCLUDE_DIR, 0, 0, 1, 0, 0},
#else
#define INCLUDE_DEFAULTS_MUSL_PREFIX
#endif

#ifdef CROSS_INCLUDE_DIR
#define INCLUDE_DEFAULTS_MUSL_CROSS			\
    { CROSS_INCLUDE_DIR, "GCC", 0, 0, 0, 0},
#else
#define INCLUDE_DEFAULTS_MUSL_CROSS
#endif

#ifdef TOOL_INCLUDE_DIR
#define INCLUDE_DEFAULTS_MUSL_TOOL			\
    { TOOL_INCLUDE_DIR, "BINUTILS", 0, 1, 0, 0},
#else
#define INCLUDE_DEFAULTS_MUSL_TOOL
#endif

#ifdef NATIVE_SYSTEM_HEADER_DIR
#define INCLUDE_DEFAULTS_MUSL_NATIVE			\
    { NATIVE_SYSTEM_HEADER_DIR, 0, 0, 0, 1, 2 },	\
    { NATIVE_SYSTEM_HEADER_DIR, 0, 0, 0, 1, 0 },
#else
#define INCLUDE_DEFAULTS_MUSL_NATIVE
#endif

#if defined (CROSS_DIRECTORY_STRUCTURE) && !defined (TARGET_SYSTEM_ROOT)
# undef INCLUDE_DEFAULTS_MUSL_LOCAL
# define INCLUDE_DEFAULTS_MUSL_LOCAL
# undef INCLUDE_DEFAULTS_MUSL_NATIVE
# define INCLUDE_DEFAULTS_MUSL_NATIVE
#else
# undef INCLUDE_DEFAULTS_MUSL_CROSS
# define INCLUDE_DEFAULTS_MUSL_CROSS
#endif

#undef INCLUDE_DEFAULTS
#define INCLUDE_DEFAULTS				\
  {							\
    INCLUDE_DEFAULTS_MUSL_GPP				\
    INCLUDE_DEFAULTS_MUSL_LOCAL				\
    INCLUDE_DEFAULTS_MUSL_PREFIX			\
    INCLUDE_DEFAULTS_MUSL_CROSS				\
    INCLUDE_DEFAULTS_MUSL_TOOL				\
    INCLUDE_DEFAULTS_MUSL_NATIVE			\
    { GCC_INCLUDE_DIR, "GCC", 0, 1, 0, 0 },		\
    { 0, 0, 0, 0, 0, 0 }				\
  }
