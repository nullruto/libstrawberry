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

#ifndef __SB_CRYPTO_BLOCKMODE_H
#define __SB_CRYPTO_BLOCKMODE_H


#include "../core/stdincl.h"


#define SB_CRYPTO_BLOCKMODE_CBC				1

#define SB_CRYPTO_BLOCKFLAG_EXPLICIT_IV		SB_BIT_16


typedef struct sb_crypto_blockmode_ctx {
	sb_size_t iv_size;
	uint16_t mode;
	uint16_t flags;
	uint8_t iv[];
} sb_crypto_blockmode_ctx_t;


#ifdef __cplusplus
extern "C" {
#endif

	SBAPI sb_bool_t sb_crypto_blockmode_init(sb_crypto_blockmode_ctx_t *ctx, uint16_t mode, uint16_t flags, sb_size_t iv_size);
	SBAPI sb_bool_t sb_crypto_blockmode_clear(sb_crypto_blockmode_ctx_t *ctx);

#ifdef __cplusplus
}
#endif


#endif
