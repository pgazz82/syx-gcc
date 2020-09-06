/* Target definitions for GCC for Intel 80386 using ELF
   Copyright (C) 1988-2019 Free Software Foundation, Inc.

   Derived from sysv4.h written by Ron Guilmette (rfg@netcom.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* The ELF ABI for the i386 says that records and unions are returned
   in memory.  */

#define SUBTARGET_RETURN_IN_MEMORY(TYPE, FNTYPE) \
	(TYPE_MODE (TYPE) == BLKmode \
	 || (VECTOR_MODE_P (TYPE_MODE (TYPE)) && int_size_in_bytes (TYPE) == 8))

#undef CPP_SPEC
#define CPP_SPEC ""

#define ENDFILE_SPEC "crtend.o%s"

#define STARTFILE_SPEC "%{!shared: %{!symbolic: %{!p:Scrt1.o%s}}} crtbegin.o%s"

#undef DBX_REGISTER_NUMBER
#define DBX_REGISTER_NUMBER(n) \
  (TARGET_64BIT ? dbx64_register_map[n] : svr4_dbx_register_map[n])

/* The routine used to output sequences of byte values.  We use a special
   version of this for most svr4 targets because doing so makes the
   generated assembly code more compact (and thus faster to assemble)
   as well as more readable.  Note that if we find subparts of the
   character sequence which end with NUL (and which are shorter than
   ELF_STRING_LIMIT) we output those using ASM_OUTPUT_LIMITED_STRING.  */

#undef ASM_OUTPUT_ASCII
#define ASM_OUTPUT_ASCII(FILE, STR, LENGTH)				\
  do									\
    {									\
      const unsigned char *_ascii_bytes =				\
        (const unsigned char *) (STR);					\
      const unsigned char *limit = _ascii_bytes + (LENGTH);		\
      unsigned bytes_in_chunk = 0;					\
      for (; _ascii_bytes < limit; _ascii_bytes++)			\
	{								\
	  const unsigned char *p;					\
	  if (bytes_in_chunk >= 64)					\
	    {								\
	      fputc ('\n', (FILE));					\
	      bytes_in_chunk = 0;					\
	    }								\
	  for (p = _ascii_bytes; p < limit && *p != '\0'; p++)		\
	    continue;							\
	  if (p < limit && (p - _ascii_bytes) <= (long) ELF_STRING_LIMIT) \
	    {								\
	      if (bytes_in_chunk > 0)					\
		{							\
		  fputc ('\n', (FILE));					\
		  bytes_in_chunk = 0;					\
		}							\
	      ASM_OUTPUT_LIMITED_STRING ((FILE), (const char *) _ascii_bytes); \
	      _ascii_bytes = p;						\
	    }								\
	  else								\
	    {								\
	      if (bytes_in_chunk == 0)					\
		fputs (ASM_BYTE, (FILE));				\
	      else							\
		fputc (',', (FILE));					\
	      fprintf ((FILE), "0x%02x", *_ascii_bytes);			\
	      bytes_in_chunk += 5;					\
	    }								\
	}								\
      if (bytes_in_chunk > 0)						\
	fputc ('\n', (FILE));						\
    }									\
  while (0)

#define LOCAL_LABEL_PREFIX	"."

/* Switch into a generic section.  */
#define TARGET_ASM_NAMED_SECTION  default_elf_asm_named_section

#undef BSS_SECTION_ASM_OP
#define BSS_SECTION_ASM_OP "\t.section\t.bss"

#undef ASM_OUTPUT_ALIGNED_BSS
#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGN) \
  asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGN)

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
