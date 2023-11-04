/*
 * Common definitions for MIPS running Linux with ELF binaries
 *
 * Copyright (C) 1994, 1995, 1996, 1998 Free Software Foundation, Inc.
 *
 * Written by Ralf Baechle (ralf@gnu.org)
 *
 * Linux is a POSIX.1 compatible UNIX clone for MIPS, which uses GNU
 * stuffs as the native stuffs.
 */

#ifndef TARGET_DEFAULT
#define TARGET_DEFAULT (MASK_ABICALLS|MASK_GAS)
#define MIPS_ISA_DEFAULT 1
#define MIPS_ABI_DEFAULT ABI_32
#endif

#ifndef TARGET_CPU_DEFAULT
#define TARGET_CPU_DEFAULT 0
#endif

#ifndef TARGET_ENDIAN_DEFAULT
#define TARGET_ENDIAN_DEFAULT MASK_BIG_ENDIAN
#endif

#ifndef MACHINE_TYPE
#define MACHINE_TYPE "(MIPS Linux/ELF)"
#endif

/*
 * -G is incompatible with -KPIC which is the default, so only allow objects
 * in the small data section if the user explicitly asks for it.
 */
#undef MIPS_DEFAULT_GVALUE
#define MIPS_DEFAULT_GVALUE 0

/*
 * Specify predefined symbols in preprocessor.
 */
#ifndef CPP_PREDEFINES
#define CPP_PREDEFINES "\
-D__ELF__ -D__PIC__ -D__pic__ -Dunix -Dmips -DR3000 -DMIPSEB -Dlinux \
-Asystem(linux) -Asystem(posix) -Acpu(mips) -Amachine(mips)"
#endif

#define SUBTARGET_CPP_SPEC "\
%{!mfp32: %{!mfp64: -D_MIPS_FPSET=16}} \
%{mfp32: -D_MIPS_FPSET=16} \
%{mfp64: -D_MIPS_FPSET=32} \
%{mips1: -D_MIPS_ISA=_MIPS_ISA_MIPS1} \
%{mips2: -D_MIPS_ISA=_MIPS_ISA_MIPS2} \
%{mips3: -D_MIPS_ISA=_MIPS_ISA_MIPS3} \
%{mips4: -D_MIPS_ISA=_MIPS_ISA_MIPS4} \
%{!mips1: %{!mips2: %{!mips3: %{!mips4: -D_MIPS_ISA=_MIPS_ISA_MIPS1}}}} \
%{mabi=32: -D_MIPS_SIM=_MIPS_SIM_ABI32} \
%{mabi=n32: -D_ABIN32=2 -D_MIPS_SIM=_ABIN32} \
%{mabi=64: -D_ABI64=3 -D_MIPS_SIM=_ABI64} \
%{!mabi*: -D_MIPS_SIM=_MIPS_SIM_ABI32} \
%{mint64:-D_MIPS_SZINT=64 %{!mlong64:-D__SIZE_TYPE__=long\\ unsigned\\ int -D__SSIZE_TYPE__=long\\ int -D__PTRDIFF_TYPE__=long\\ int -D_MIPS_SZLONG=64 -D_MIPS_SZPTR=64}} \
%{!mint64:-D_MIPS_SZINT=32 %{!mlong64:-D__SIZE_TYPE__=unsigned\\ int -D__SSIZE_TYPE__=int -D__PTRDIFF_TYPE__=int -D_MIPS_SZLONG=32 -D_MIPS_SZPTR=32}} \
%{mlong64:-D__SIZE_TYPE__=long\\ unsigned\\ int -D__SSIZE_TYPE__=long\\ int -D__PTRDIFF_TYPE__=long\\ int -D_MIPS_SZLONG=64 -D_MIPS_SZPTR=64} \
%{mabi=32: -D_MIPS_SZLONG=32} \
%{mabi=n32: -D_MIPS_SZLONG=32} \
%{mabi=64: -D_MIPS_SZLONG=64} \
%{!mabi*: -D_MIPS_SZLONG=32} \
%{mabi=32: -D_MIPS_SZPTR=32} \
%{mabi=n32: -D_MIPS_SZPTR=32} \
%{mabi=64: -D_MIPS_SZPTR=64} \
%{!mabi*: -D_MIPS_SZPTR=32} \
%{!mips*: -U__mips -D__mips} \
%{mabi=32: -U__mips64} \
%{mabi=n32: -D__mips64} \
%{mabi=64: -D__mips64} \
%{!mabi*: -U__mips64} \
%{fno-PIC:-U__PIC__ -U__pic__} %{fno-pic:-U__PIC__ -U__pic__} \
%{fPIC:-D__PIC__ -D__pic__} %{fpic:-D__PIC__ -D__pic__} \
%{-D__HAVE_FPU__ } \
%{posix:-D_POSIX_SOURCE} \
%{pthread:-D_REENTRANT}"

/*
 * Extra switches sometimes passed to the assembler.
 */
#define SUBTARGET_ASM_SPEC "\
%{!fno-PIC:%{!fno-pic:-KPIC}} %{fPIC:-KPIC} %{fpic:-KPIC} \
%{fno-PIC:-non_shared} %{fno-pic:-non_shared}"

/* Provide a STARTFILE_SPEC appropriate for Linux.  Here we add
   the Linux magical crtbegin.o file (see crtstuff.c) which
   provides part of the support for getting C++ file-scope static
   object constructed before entering `main'. */

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC \
  "%{!shared: \
     %{pg:gcrt1.o%s} %{!pg:%{p:gcrt1.o%s} \
                       %{!p:%{profile:gcrt1.o%s} \
                         %{!profile:crt1.o%s}}}} \
   crti.o%s %{!shared:crtbegin.o%s} %{shared:crtbeginS.o%s}"

/* Provide a ENDFILE_SPEC appropriate for Linux.  Here we tack on
   the Linux magical crtend.o file (see crtstuff.c) which
   provides part of the support for getting C++ file-scope static
   object constructed before entering `main', followed by a normal
   Linux "finalizer" file, `crtn.o'.  */

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC \
  "%{!shared:crtend.o%s} %{shared:crtendS.o%s} crtn.o%s"

/* Provide a LINK_SPEC appropriate for Linux.  Here we provide support
   for the special GCC options -static and -shared, which allow us to
   link things in one of these three modes by applying the appropriate
   combinations of options at link-time. We like to support here for
   as many of the other GNU linker options as possible. But I don't
   have the time to search for those flags. I am sure how to add
   support for -soname shared_object_name. H.J.

   I took out %{v:%{!V:-V}}. It is too much :-(. They can use
   -Wl,-V.

   When the -shared link option is used a final link is not being
   done.  */

#undef	LINK_SPEC
#define LINK_SPEC "\
%{G*} %{EB} %{EL} %{mips1} %{mips2} %{mips3} %{mips4} \
%{bestGnum} %{shared} %{non_shared} \
%{call_shared} %{no_archive} %{exact_version} \
  %{!shared: \
      %{!static: \
	%{rdynamic:-export-dynamic} \
	%{!dynamic-linker:-dynamic-linker /lib/ld.so.1}} \
	%{static:-static}}"

/*
 * We no longer link with libc_p.a or libg.a by default. If you
 * want to profile or debug the Linux C library, please add
 * -lc_p or -ggdb to LDFLAGS at the link time, respectively.
 */
#undef	LIB_SPEC
#define LIB_SPEC \
  "%{shared: -lc} \
   %{!shared: %{mieee-fp:-lieee} %{pthread:-lpthread} \
        %{profile:-lc_p} %{!profile: -lc}}"

#include "mips/mips.h"

/* mips/mips.h defines some defaults which are quite stupid ...  */

/* Do not define this macro if `MULTILIB_OPTIONS' is not defined in the target
   makefile fragment or if none of the options listed in `MULTILIB_OPTIONS'
   are set by default.  */
#undef MULTILIB_DEFAULTS

#undef OBJECT_FORMAT_COFF
#undef EXTENDED_COFF
#define OBJECT_FORMAT_ELF

/* Don't assume anything about the header files. */
#define NO_IMPLICIT_EXTERN_C

#undef HAVE_ATEXIT
#define HAVE_ATEXIT

/* Elf can do stabs */
#define SDB_DEBUGGING_INFO

/* Prefer dbx.  */
#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG

/* Generate calls to memcpy, etc., not bcopy, etc. */
#define TARGET_MEM_FUNCTIONS

/* Output the label which precedes a jumptable.  Note that for all svr4
   systems where we actually generate jumptables (which is to say every
   svr4 target except i386, where we use casesi instead) we put the jump-
   tables into the .rodata section and since other stuff could have been
   put into the .rodata section prior to any given jumptable, we have to
   make sure that the location counter for the .rodata section gets pro-
   perly re-aligned prior to the actual beginning of the jump table.  */

#define ASM_OUTPUT_BEFORE_CASE_LABEL(FILE,PREFIX,NUM,TABLE) \
  ASM_OUTPUT_ALIGN((FILE), 2)

#undef ASM_OUTPUT_CASE_LABEL
#define ASM_OUTPUT_CASE_LABEL(FILE,PREFIX,NUM,JUMPTABLE)          \
  do {                                                            \
    ASM_OUTPUT_BEFORE_CASE_LABEL (FILE, PREFIX, NUM, JUMPTABLE);  \
    ASM_OUTPUT_INTERNAL_LABEL (FILE, PREFIX, NUM);                \
  } while (0)

/* This says how to output assembler code to declare an
   uninitialized external linkage data object.  Under SVR4,
   the linker seems to want the alignment of data objects
   to depend on their types.  We do exactly that here.  */

#define COMMON_ASM_OP	".comm"

#define ASM_OUTPUT_ALIGNED_COMMON(FILE, NAME, SIZE, ALIGN)		\
do {									\
  fprintf ((FILE), "\t%s\t", COMMON_ASM_OP);				\
  assemble_name ((FILE), (NAME));					\
  fprintf ((FILE), ",%u,%u\n", (SIZE), (ALIGN) / BITS_PER_UNIT);	\
} while (0)

/* This says how to output assembler code to declare an
   uninitialized internal linkage data object.  Under SVR4,
   the linker seems to want the alignment of data objects
   to depend on their types.  We do exactly that here.  */

#define LOCAL_ASM_OP	".local"

#define ASM_OUTPUT_ALIGNED_LOCAL(FILE, NAME, SIZE, ALIGN)		\
do {									\
  fprintf ((FILE), "\t%s\t", LOCAL_ASM_OP);				\
  assemble_name ((FILE), (NAME));					\
  fprintf ((FILE), "\n");						\
  ASM_OUTPUT_ALIGNED_COMMON (FILE, NAME, SIZE, ALIGN);			\
} while (0)

/* Output at beginning of assembler file.  */
/* The .file command should always begin the output.  */
#undef ASM_FILE_START
#define ASM_FILE_START(FILE)						\
  do {									\
	output_file_directive (FILE, main_input_filename);		\
	fprintf (FILE, "\t.version\t\"01.01\"\n");			\
  } while (0)

/* This is the pseudo-op used to generate a 32-bit word of data with a
   specific value in some section.  This is the same for all known svr4
   assemblers.  */

#define INT_ASM_OP		".long"

/* This is the pseudo-op used to generate a contiguous sequence of byte
   values from a double-quoted string WITHOUT HAVING A TERMINATING NUL
   AUTOMATICALLY APPENDED.  This is the same for most svr4 assemblers.  */

#define ASCII_DATA_ASM_OP	".ascii"


/* Support const sections and the ctors and dtors sections for g++.
   Note that there appears to be two different ways to support const
   sections at the moment.  You can either #define the symbol
   READONLY_DATA_SECTION (giving it some code which switches to the
   readonly data section) or else you can #define the symbols
   EXTRA_SECTIONS, EXTRA_SECTION_FUNCTIONS, SELECT_SECTION, and
   SELECT_RTX_SECTION.  We do both here just to be on the safe side.  */

#define USE_CONST_SECTION	1

#define CONST_SECTION_ASM_OP_32 "\t.rdata"
#define CONST_SECTION_ASM_OP_64 ".section\t.rodata"
/* Silence GCC when using this file for the Linux libc */
#undef CTORS_SECTION_ASM_OP
#undef DTORS_SECTION_ASM_OP
#define CTORS_SECTION_ASM_OP    ".section\t.ctors,\"aw\""
#define DTORS_SECTION_ASM_OP    ".section\t.dtors,\"aw\""

/* A default list of other sections which we might be "in" at any given
   time.  For targets that use additional sections (e.g. .tdesc) you
   should override this definition in the target-specific file which
   includes this file.  */

#undef EXTRA_SECTIONS
#define EXTRA_SECTIONS in_sdata, in_rdata, in_const, in_ctors, in_dtors

/* A default list of extra section function definitions.  For targets
   that use additional sections (e.g. .tdesc) you should override this
   definition in the target-specific file which includes this file.  */

#undef EXTRA_SECTION_FUNCTIONS
#define EXTRA_SECTION_FUNCTIONS                                         \
void                                                                    \
sdata_section ()                                                        \
{                                                                       \
  if (in_section != in_sdata)                                           \
    {                                                                   \
      fprintf (asm_out_file, "%s\n", SDATA_SECTION_ASM_OP);             \
      in_section = in_sdata;                                            \
    }                                                                   \
}                                                                       \
                                                                        \
void									\
const_section ()							\
{									\
  if (!USE_CONST_SECTION)						\
    text_section();							\
  else if (in_section != in_const)					\
    {									\
      if (mips_isa >= 3)                                                \
        fprintf (asm_out_file, "%s\n", CONST_SECTION_ASM_OP_64);        \
      else                                                              \
        fprintf (asm_out_file, "%s\n", CONST_SECTION_ASM_OP_32);        \
      in_section = in_const;						\
    }									\
}                                                                       \
                                                                        \
void                                                                    \
rdata_section ()                                                        \
{                                                                       \
  if (in_section != in_rdata)                                           \
    {                                                                   \
      if (mips_isa >= 3)                                                \
        fprintf (asm_out_file, "%s\n", CONST_SECTION_ASM_OP_64);        \
      else                                                              \
        fprintf (asm_out_file, "%s\n", CONST_SECTION_ASM_OP_32);        \
      in_section = in_rdata;                                            \
    }                                                                   \
}                                                                       \
  CTORS_SECTION_FUNCTION						\
  DTORS_SECTION_FUNCTION

#define CTORS_SECTION_FUNCTION						\
void									\
ctors_section ()							\
{									\
  if (in_section != in_ctors)						\
    {									\
      fprintf (asm_out_file, "%s\n", CTORS_SECTION_ASM_OP);		\
      in_section = in_ctors;						\
    }									\
}

#define DTORS_SECTION_FUNCTION						\
void									\
dtors_section ()							\
{									\
  if (in_section != in_dtors)						\
    {									\
      fprintf (asm_out_file, "%s\n", DTORS_SECTION_ASM_OP);		\
      in_section = in_dtors;						\
    }									\
}

/* A C statement (sans semicolon) to output an element in the table of
   global constructors.  */
#define ASM_OUTPUT_CONSTRUCTOR(FILE,NAME)				\
  do {									\
    ctors_section ();							\
    fprintf (FILE, "\t%s\t ", INT_ASM_OP);				\
    assemble_name (FILE, NAME);						\
    fprintf (FILE, "\n");						\
  } while (0)

/* A C statement (sans semicolon) to output an element in the table of
   global destructors.  */
#define ASM_OUTPUT_DESTRUCTOR(FILE,NAME)       				\
  do {									\
    dtors_section ();                   				\
    fprintf (FILE, "\t%s\t ", INT_ASM_OP);				\
    assemble_name (FILE, NAME);              				\
    fprintf (FILE, "\n");						\
  } while (0)


#define INIT_SECTION_ASM_OP ".section .init,\"x\""
#define FINI_SECTION_ASM_OP ".section .fini,\"x\""

/* Switch into a generic section.
 
   We make the section read-only and executable for a function decl,
   read-only for a const data decl, and writable for a non-const data decl.
 
   If the section has already been defined, we must not
   emit the attributes here. The SVR4 assembler does not
   recognize section redefinitions.
   If DECL is NULL, no attributes are emitted.  */

#define ASM_OUTPUT_SECTION_NAME(FILE, DECL, NAME, RELOC)		\
do {									\
  static struct section_info						\
    {									\
      struct section_info *next;				        \
      char *name;						        \
      enum sect_enum {SECT_RW, SECT_RO, SECT_EXEC} type;		\
    } *sections;							\
  struct section_info *s;						\
  char *mode;								\
  enum sect_enum type;							\
									\
  for (s = sections; s; s = s->next)					\
    if (!strcmp (NAME, s->name))					\
      break;								\
									\
  if (DECL && TREE_CODE (DECL) == FUNCTION_DECL)			\
    type = SECT_EXEC, mode = "ax";					\
  else if (DECL && DECL_READONLY_SECTION (DECL, RELOC))			\
    type = SECT_RO, mode = "a";						\
  else									\
    type = SECT_RW, mode = "aw";					\
									\
  if (s == 0)								\
    {									\
      s = (struct section_info *) xmalloc (sizeof (struct section_info));  \
      s->name = xmalloc ((strlen (NAME) + 1) * sizeof (*NAME));		\
      strcpy (s->name, NAME);						\
      s->type = type;							\
      s->next = sections;						\
      sections = s;							\
      fprintf (FILE, ".section\t%s,\"%s\",@progbits\n", NAME, mode);	\
    }									\
  else									\
    {									\
      if (DECL && s->type != type)					\
	error_with_decl (DECL, "%s causes a section type conflict");	\
									\
      fprintf (FILE, ".section\t%s\n", NAME);				\
    }									\
} while (0)


/* A C statement or statements to switch to the appropriate
   section for output of DECL.  DECL is either a `VAR_DECL' node
   or a constant of some sort.  RELOC indicates whether forming
   the initial value of DECL requires link-time relocations.  */

#undef SELECT_SECTION
#define SELECT_SECTION(DECL,RELOC)					\
{									\
  if (TREE_CODE (DECL) == STRING_CST)					\
    {									\
      if (! flag_writable_strings)					\
	const_section ();						\
      else								\
	data_section ();						\
    }									\
  else if (TREE_CODE (DECL) == VAR_DECL)				\
    {									\
      if ((flag_pic && RELOC)						\
	  || !TREE_READONLY (DECL) || TREE_SIDE_EFFECTS (DECL)		\
	  || !DECL_INITIAL (DECL)					\
	  || (DECL_INITIAL (DECL) != error_mark_node			\
	      && !TREE_CONSTANT (DECL_INITIAL (DECL))))			\
	data_section ();						\
      else								\
	const_section ();						\
    }									\
  else									\
    const_section ();							\
}

/* A C statement or statements to switch to the appropriate
   section for output of RTX in mode MODE.  RTX is some kind
   of constant in RTL.  The argument MODE is redundant except
   in the case of a `const_int' rtx.  Currently, these always
   go into the const section.  */

#undef SELECT_RTX_SECTION
#define SELECT_RTX_SECTION(MODE,RTX) const_section()

/* Define the strings used for the special svr4 .type and .size directives.
   These strings generally do not vary from one system running svr4 to
   another, but if a given system (e.g. m88k running svr) needs to use
   different pseudo-op names for these, they may be overridden in the
   file which includes this one.  */

#define TYPE_ASM_OP	".type"
#define SIZE_ASM_OP	".size"

/* This is how we tell the assembler that a symbol is weak.  */

#define ASM_WEAKEN_LABEL(FILE,NAME) \
  do { fputs ("\t.weak\t", FILE); assemble_name (FILE, NAME); \
       fputc ('\n', FILE); } while (0)

/*
 * This is how to equate one symbol to another symbol.  The syntax used is
 * `SYM1=SYM2'.  Note that this is different from the way equates are done
 * with most svr4 assemblers, where the syntax is `.set SYM1,SYM2'.
 */
#define ASM_OUTPUT_DEF(FILE,LABEL1,LABEL2)				\
 do {	fprintf ((FILE), "\t");						\
	assemble_name (FILE, LABEL1);					\
	fprintf (FILE, " = ");						\
	assemble_name (FILE, LABEL2);					\
	fprintf (FILE, "\n");						\
  } while (0)

/* The following macro defines the format used to output the second
   operand of the .type assembler directive.  Different svr4 assemblers
   expect various different forms for this operand.  The one given here
   is just a default.  You may need to override it in your machine-
   specific tm.h file (depending upon the particulars of your assembler).  */

#define TYPE_OPERAND_FMT	"@%s"

/* These macros generate the special .type and .size directives which
   are used to set the corresponding fields of the linker symbol table
   entries in an ELF object file under SVR4.  These macros also output
   the starting labels for the relevant functions/objects.  */

/* Write the extra assembler code needed to declare a function properly.  */

#undef ASM_DECLARE_FUNCTION_NAME
#define ASM_DECLARE_FUNCTION_NAME(FILE, NAME, DECL)			\
  do {									\
    fprintf (FILE, "\t%s\t ", TYPE_ASM_OP);				\
    assemble_name (FILE, NAME);						\
    putc (',', FILE);							\
    fprintf (FILE, TYPE_OPERAND_FMT, "function");			\
    putc ('\n', FILE);							\
    ASM_OUTPUT_LABEL(FILE, NAME);					\
  } while (0)

/* Write the extra assembler code needed to declare an object properly.  */

#undef ASM_DECLARE_OBJECT_NAME
#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL)			\
  do {									\
    fprintf (FILE, "\t%s\t ", TYPE_ASM_OP);				\
    assemble_name (FILE, NAME);						\
    putc (',', FILE);							\
    fprintf (FILE, TYPE_OPERAND_FMT, "object");				\
    putc ('\n', FILE);							\
    size_directive_output = 0;						\
    if (!flag_inhibit_size_directive && DECL_SIZE (DECL))		\
      {									\
	size_directive_output = 1;					\
	fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);				\
	assemble_name (FILE, NAME);					\
	fprintf (FILE, ",%d\n",  int_size_in_bytes (TREE_TYPE (DECL)));	\
      }									\
    ASM_OUTPUT_LABEL(FILE, NAME);					\
  } while (0)

/* Output the size directive for a decl in rest_of_decl_compilation
   in the case where we did not do so before the initializer.
   Once we find the error_mark_node, we know that the value of
   size_directive_output was set
   by ASM_DECLARE_OBJECT_NAME when it was run for the same decl.  */

#undef ASM_FINISH_DECLARE_OBJECT
#define ASM_FINISH_DECLARE_OBJECT(FILE, DECL, TOP_LEVEL, AT_END)	 \
do {									 \
     char *name = XSTR (XEXP (DECL_RTL (DECL), 0), 0);			 \
     if (!flag_inhibit_size_directive && DECL_SIZE (DECL)		 \
         && ! AT_END && TOP_LEVEL					 \
	 && DECL_INITIAL (DECL) == error_mark_node			 \
	 && !size_directive_output)					 \
       {								 \
	 size_directive_output = 1;					 \
	 fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);			 \
	 assemble_name (FILE, name);					 \
	 fprintf (FILE, ",%d\n",  int_size_in_bytes (TREE_TYPE (DECL))); \
       }								 \
   } while (0)

/*
 * This is how to declare the size of a function.
 * mips.h doesn't declare this and the one in svr4.h we just imported
 * will produce bad code for GP optimized code.
 */
#define ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)			\
  do {									\
    if (!flag_inhibit_size_directive)					\
      {									\
        char label[256];						\
	static int labelno;						\
	labelno++;							\
									\
	ASM_GENERATE_INTERNAL_LABEL (label, "Lfe", labelno);		\
	ASM_OUTPUT_INTERNAL_LABEL (FILE, "Lfe", labelno);	        \
	fprintf (FILE, "\t%s\t ", SIZE_ASM_OP);		                \
	assemble_name (FILE, (FNAME));			                \
        fprintf (FILE, ",");				                \
	assemble_name (FILE, label);			                \
        fprintf (FILE, "-");				                \
	assemble_name (FILE, (FNAME));			                \
	putc ('\n', FILE);					        \
      }									\
  } while (0)

#undef ASM_FILE_END
#define ASM_FILE_END(FILE)						\
  do {				 					\
	mips_asm_file_end(FILE);					\
	fprintf ((FILE), "\t%s\t\"GCC: (GNU) %s\"\n",			\
		IDENT_ASM_OP, version_string);				\
  } while (0)


/* The default ident must be replaced.  */
#undef ASM_OUTPUT_IDENT

/* Attach a special .ident directive to the end of the file to identify
   the version of GCC which compiled this code.  The format of the
   .ident string is patterned after the ones produced by native svr4
   C compilers.  */

#define IDENT_ASM_OP ".ident"

/* Output #ident as a .ident.  */

#define ASM_OUTPUT_IDENT(FILE, NAME) \
  fprintf (FILE, "\t%s\t\"%s\"\n", IDENT_ASM_OP, NAME);


/* A table of bytes codes used by the ASM_OUTPUT_ASCII and
   ASM_OUTPUT_LIMITED_STRING macros.  Each byte in the table
   corresponds to a particular byte value [0..255].  For any
   given byte value, if the value in the corresponding table
   position is zero, the given character can be output directly.
   If the table value is 1, the byte must be output as a \ooo
   octal escape.  If the tables value is anything else, then the
   byte value should be output as a \ followed by the value
   in the table.  Note that we can use standard UN*X escape
   sequences for many control characters, but we don't use
   \a to represent BEL because some svr4 assemblers (e.g. on
   the i386) don't know about that.  Also, we don't use \v
   since some versions of gas, such as 2.2 did not accept it.  */

#define ESCAPES \
"\1\1\1\1\1\1\1\1btn\1fr\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\
\0\0\"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\\\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\
\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\
\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\
\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\
\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"

/* Some svr4 assemblers have a limit on the number of characters which
   can appear in the operand of a .string directive.  If your assembler
   has such a limitation, you should define STRING_LIMIT to reflect that
   limit.  Note that at least some svr4 assemblers have a limit on the
   actual number of bytes in the double-quoted string, and that they
   count each character in an escape sequence as one byte.  Thus, an
   escape sequence like \377 would count as four bytes.

   If your target assembler doesn't support the .string directive, you
   should define this to zero.
*/

#define STRING_LIMIT	((unsigned) 256)

#define STRING_ASM_OP	".string"

/* The routine used to output NUL terminated strings.  We use a special
   version of this for most svr4 targets because doing so makes the
   generated assembly code more compact (and thus faster to assemble)
   as well as more readable, especially for targets like the i386
   (where the only alternative is to output character sequences as
   comma separated lists of numbers).   */

#define ASM_OUTPUT_LIMITED_STRING(FILE, STR)				\
  do									\
    {									\
      register unsigned char *_limited_str = (unsigned char *) (STR);	\
      register unsigned ch;						\
      fprintf ((FILE), "\t%s\t\"", STRING_ASM_OP);			\
      for (; ch = *_limited_str; _limited_str++)			\
        {								\
	  register int escape;						\
	  switch (escape = ESCAPES[ch])					\
	    {								\
	    case 0:							\
	      putc (ch, (FILE));					\
	      break;							\
	    case 1:							\
	      fprintf ((FILE), "\\%03o", ch);				\
	      break;							\
	    default:							\
	      putc ('\\', (FILE));					\
	      putc (escape, (FILE));					\
	      break;							\
	    }								\
        }								\
      fprintf ((FILE), "\"\n");						\
    }									\
  while (0)

/* The routine used to output sequences of byte values.  We use a special
   version of this for most svr4 targets because doing so makes the
   generated assembly code more compact (and thus faster to assemble)
   as well as more readable.  Note that if we find subparts of the
   character sequence which end with NUL (and which are shorter than
   STRING_LIMIT) we output those using ASM_OUTPUT_LIMITED_STRING.  */

#undef ASM_OUTPUT_ASCII
#define ASM_OUTPUT_ASCII(FILE, STR, LENGTH)				\
  do									\
    {									\
      register unsigned char *_ascii_bytes = (unsigned char *) (STR);	\
      register unsigned char *limit = _ascii_bytes + (LENGTH);		\
      register unsigned bytes_in_chunk = 0;				\
      for (; _ascii_bytes < limit; _ascii_bytes++)			\
        {								\
	  register unsigned char *p;					\
	  if (bytes_in_chunk >= 60)					\
	    {								\
	      fprintf ((FILE), "\"\n");					\
	      bytes_in_chunk = 0;					\
	    }								\
	  for (p = _ascii_bytes; p < limit && *p != '\0'; p++)		\
	    continue;							\
	  if (p < limit && (p - _ascii_bytes) <= STRING_LIMIT)		\
	    {								\
	      if (bytes_in_chunk > 0)					\
		{							\
		  fprintf ((FILE), "\"\n");				\
		  bytes_in_chunk = 0;					\
		}							\
	      ASM_OUTPUT_LIMITED_STRING ((FILE), _ascii_bytes);		\
	      _ascii_bytes = p;						\
	    }								\
	  else								\
	    {								\
	      register int escape;					\
	      register unsigned ch;					\
	      if (bytes_in_chunk == 0)					\
		fprintf ((FILE), "\t%s\t\"", ASCII_DATA_ASM_OP);	\
	      switch (escape = ESCAPES[ch = *_ascii_bytes])		\
		{							\
		case 0:							\
		  putc (ch, (FILE));					\
		  bytes_in_chunk++;					\
		  break;						\
		case 1:							\
		  fprintf ((FILE), "\\%03o", ch);			\
		  bytes_in_chunk += 4;					\
		  break;						\
		default:						\
		  putc ('\\', (FILE));					\
		  putc (escape, (FILE));				\
		  bytes_in_chunk += 2;					\
		  break;						\
		}							\
	    }								\
	}								\
      if (bytes_in_chunk > 0)						\
        fprintf ((FILE), "\"\n");					\
    }									\
  while (0)

#undef DEFAULT_VTABLE_THUNKS
#define DEFAULT_VTABLE_THUNKS 1

/* Get rid of SysV garbage imported via mips/mips.h.  */

#undef MD_EXEC_PREFIX
#undef MD_STARTFILE_PREFIX

/* Specify wchar_t types.  */

#undef	WCHAR_TYPE
#define WCHAR_TYPE	"long int"

#undef	WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE	LONG_TYPE_SIZE

#undef	MAX_WCHAR_TYPE_SIZE
#define MAX_WCHAR_TYPE_SIZE	MAX_LONG_TYPE_SIZE

/* Biggest alignment supported by the object file format of this
   machine.  Use this macro to limit the alignment which can be
   specified using the `__attribute__ ((aligned (N)))' construct.  If
   not defined, the default value is `BIGGEST_ALIGNMENT'.  */

#define MAX_OFILE_ALIGNMENT (32768*8)
