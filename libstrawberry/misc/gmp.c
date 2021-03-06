/*******************************************************************************
**                                                                            **
**   The MIT License                                                          **
**                                                                            **
**   Copyright 2016 strawberryentrypoint                                      **
**                                                                            **
**   Permission is hereby granted, free of charge, to any person              **
**   obtaining a copy of this software and associated documentation files     **
**   (the "Software"), to deal in the Software without restriction,           **
**   including without limitation the rights to use, copy, modify, merge,     **
**   publish, distribute, sublicense, and/or sell copies of the Software,     **
**   and to permit persons to whom the Software is furnished to do so,        **
**   subject to the following conditions:                                     **
**                                                                            **
**   The above copyright notice and this permission notice shall be           **
**   included in all copies or substantial portions of the Software.          **
**                                                                            **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   **
**                                                                            **
********************************************************************************
**
**  Notes:
**    -
**
*/

#define __FILE_LOCAL__						"misc/gmp.c"

#if HAVE_GMP

#include "./gmp.h"

#include "../core/poison.h"


IDENTID(__FILE_LOCAL__, "0.1", "1", "2016-09-04");


void sb_mpz_import_ex(mpz_t out, void *in, sb_size_t length, uint32_t wordorder, sb_size_t wordsize, uint32_t endianness, sb_size_t nails) {
	mpz_import(out, length, wordorder, wordsize, endianness, 0, in);
}


void* sb_mpz_export_ex(void *out, mpz_t in, void *countp, uint32_t wordorder, sb_size_t wordsize, uint32_t endianness, sb_size_t nails) {
	return mpz_export(out, NULL, wordorder, wordsize, endianness, 0, in);
}

#else
#	pragma message("Excluded: "__FILE_LOCAL__)
#endif
