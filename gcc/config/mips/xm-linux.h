/*
 * Configuration for GCC for Linux/MIPS64.
 *
 * Contributed by Ralf Baechle (ralf@gnu.org)
 */
#include <mips/xm-mips.h>
#include <xm-linux.h>

#undef HOST_BITS_PER_LONG
#define HOST_BITS_PER_LONG	_MIPS_SZLONG
#undef HOST_BITS_PER_INT
#define HOST_BITS_PER_INT	_MIPS_SZINT
