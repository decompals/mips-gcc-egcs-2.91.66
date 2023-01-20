/*
 * Definitions for little endian Linux/MIPS64 running ELF binaries
 *
 * Copyright (C) 1999 Free Software Foundation, Inc.
 *
 * Contributed by Ralf Baechle (ralf@gnu.org)
 *
 * Linux is a POSIX.1 compatible UNIX clone for MIPS, which uses GNU
 * stuffs as the native stuffs.
 */

#define TARGET_ENDIAN_DEFAULT 0

#define MACHINE_TYPE "MIPSEL Linux/ELF"

/*
 * Specify predefined symbols in preprocessor.
 */
#ifndef CPP_PREDEFINES
#define CPP_PREDEFINES "\
-D__ELF__ -Dunix -D_unix -Dlinux -Dmips -D_mips -DMIPSEL -D_MIPSEL \
-Asystem(linux) -Asystem(posix) -Acpu(mips) -Amachine(mips)"
#endif

#include "mips/linux64.h"
