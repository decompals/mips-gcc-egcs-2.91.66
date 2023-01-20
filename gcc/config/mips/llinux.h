/*
 * Definitions for little endian MIPS running Linux with ELF binary format
 *
 * Copyright (C) 1994, 1995, 1996, 1998 Free Software Foundation, Inc.
 *
 * Written by Ralf Baechle (ralf@gnu.org)
 *
 * Linux is a POSIX.1 compatible UNIX clone for MIPS, which uses GNU
 * stuffs as the native stuffs.
 */

#define TARGET_ENDIAN_DEFAULT 0

#define MACHINE_TYPE "MIPSEL Linux/ELF"

/*
 * Specify predefined symbols in preprocessor.
 */
#define CPP_PREDEFINES "\
-D__ELF__ -D__PIC__ -D__pic__ -Dunix -Dmips -DR3000 -DMIPSEL -Dlinux \
-Asystem(linux) -Asystem(posix) -Acpu(mips) -Amachine(mips)"

#include "mips/linux.h"
