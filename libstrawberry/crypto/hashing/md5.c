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

#define __FILE_LOCAL__						"crypto/hashing/md5.c"

#if !defined(SB_EXCLUDE_CRYPTO_HASHING) && !defined(SB_EXCLUDE_CRYPTO_HASHING_MD5)

#include "./md5.h"

#include "../../core/error.h"
#include "../../core/memory.h"
#include "../../core/bits.h"

#include "../../core/poison.h"


IDENTID(__FILE_LOCAL__, "0.1", "1", "2016-07-29");


#define F(x, y, z)							((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)							((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)							(((x) ^ (y)) ^ (z))
#define H2(x, y, z)							((x) ^ ((y) ^ (z)))
#define I(x, y, z)							((y) ^ ((x) | ~(z)))

#define MD5R1(a, b, c, d, x, t, s) {		\
	(a) += F((b), (c), (d)) + (x) + (t);	\
	(a) = SB_ROTL32((a), (s));				\
	(a) += (b);								\
}
#define MD5R2(a, b, c, d, x, t, s) {		\
	(a) += G((b), (c), (d)) + (x) + (t);	\
	(a) = SB_ROTL32((a), (s));				\
	(a) += (b);								\
}
#define MD5R3A(a, b, c, d, x, t, s) {		\
	(a) += H((b), (c), (d)) + (x) + (t);	\
	(a) = SB_ROTL32((a), (s));				\
	(a) += (b);								\
}
#define MD5R3B(a, b, c, d, x, t, s) {		\
	(a) += H2((b), (c), (d)) + (x) + (t);	\
	(a) = SB_ROTL32((a), (s));				\
	(a) += (b);								\
}
#define MD5R4(a, b, c, d, x, t, s) {		\
	(a) += I((b), (c), (d)) + (x) + (t);	\
	(a) = SB_ROTL32((a), (s));				\
	(a) += (b);								\
}

#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#	define MD5RSET(n) (						\
		(*(uint32_t*)&ptr[(n) * 4])			\
	)
#	define MD5RGET(n) (						\
		MD5RSET(n)							\
	)
#else
#	define MD5RSET(n) (						\
		(ctx->block[(n)] =					\
		(uint32_t)ptr[(n) * 4] |			\
		((uint32_t)ptr[(n) * 4 + 1] << 8) |	\
		((uint32_t)ptr[(n) * 4 + 2] << 16) |\
		((uint32_t)ptr[(n) * 4 + 3] << 24)) \
	)
#	define MD5RGET(n) (						\
		(ctx->block[(n)])					\
	)
#endif

static void* sb_crypto_md5_internal_update(sb_crypto_md5_ctx_t *ctx, void *data, sb_size_t size) {
	uint8_t *ptr = data;

	register uint32_t a, b, c, d;
	uint32_t saved_a, saved_b, saved_c, saved_d;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	do {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

		MD5R1 (a, b, c, d, MD5RSET( 0), 0xD76AA478U,  7);
		MD5R1 (d, a, b, c, MD5RSET( 1), 0xE8C7B756U, 12);
		MD5R1 (c, d, a, b, MD5RSET( 2), 0x242070DBU, 17);
		MD5R1 (b, c, d, a, MD5RSET( 3), 0xC1BDCEEEU, 22);
		MD5R1 (a, b, c, d, MD5RSET( 4), 0xF57C0FAFU,  7);
		MD5R1 (d, a, b, c, MD5RSET( 5), 0x4787C62AU, 12);
		MD5R1 (c, d, a, b, MD5RSET( 6), 0xA8304613U, 17);
		MD5R1 (b, c, d, a, MD5RSET( 7), 0xFD469501U, 22);
		MD5R1 (a, b, c, d, MD5RSET( 8), 0x698098D8U,  7);
		MD5R1 (d, a, b, c, MD5RSET( 9), 0x8B44F7AFU, 12);
		MD5R1 (c, d, a, b, MD5RSET(10), 0xFFFF5BB1U, 17);
		MD5R1 (b, c, d, a, MD5RSET(11), 0x895CD7BEU, 22);
		MD5R1 (a, b, c, d, MD5RSET(12), 0x6B901122U,  7);
		MD5R1 (d, a, b, c, MD5RSET(13), 0xFD987193U, 12);
		MD5R1 (c, d, a, b, MD5RSET(14), 0xA679438EU, 17);
		MD5R1 (b, c, d, a, MD5RSET(15), 0x49B40821U, 22);

		MD5R2 (a, b, c, d, MD5RGET( 1), 0xF61E2562U,  5);
		MD5R2 (d, a, b, c, MD5RGET( 6), 0xC040B340U,  9);
		MD5R2 (c, d, a, b, MD5RGET(11), 0x265E5A51U, 14);
		MD5R2 (b, c, d, a, MD5RGET( 0), 0xE9B6C7AAU, 20);
		MD5R2 (a, b, c, d, MD5RGET( 5), 0xD62F105DU,  5);
		MD5R2 (d, a, b, c, MD5RGET(10), 0x02441453U,  9);
		MD5R2 (c, d, a, b, MD5RGET(15), 0xD8A1E681U, 14);
		MD5R2 (b, c, d, a, MD5RGET( 4), 0xE7D3FBC8U, 20);
		MD5R2 (a, b, c, d, MD5RGET( 9), 0x21E1CDE6U,  5);
		MD5R2 (d, a, b, c, MD5RGET(14), 0xC33707D6U,  9);
		MD5R2 (c, d, a, b, MD5RGET( 3), 0xF4D50D87U, 14);
		MD5R2 (b, c, d, a, MD5RGET( 8), 0x455A14EDU, 20);
		MD5R2 (a, b, c, d, MD5RGET(13), 0xA9E3E905U,  5);
		MD5R2 (d, a, b, c, MD5RGET( 2), 0xFCEFA3F8U,  9);
		MD5R2 (c, d, a, b, MD5RGET( 7), 0x676F02D9U, 14);
		MD5R2 (b, c, d, a, MD5RGET(12), 0x8D2A4C8AU, 20);

		MD5R3A(a, b, c, d, MD5RGET( 5), 0xFFFA3942U,  4);
		MD5R3B(d, a, b, c, MD5RGET( 8), 0x8771F681U, 11);
		MD5R3A(c, d, a, b, MD5RGET(11), 0x6D9D6122U, 16);
		MD5R3B(b, c, d, a, MD5RGET(14), 0xFDE5380CU, 23);
		MD5R3A(a, b, c, d, MD5RGET( 1), 0xA4BEEA44U,  4);
		MD5R3B(d, a, b, c, MD5RGET( 4), 0x4BDECFA9U, 11);
		MD5R3A(c, d, a, b, MD5RGET( 7), 0xF6BB4B60U, 16);
		MD5R3B(b, c, d, a, MD5RGET(10), 0xBEBFBC70U, 23);
		MD5R3A(a, b, c, d, MD5RGET(13), 0x289B7EC6U,  4);
		MD5R3B(d, a, b, c, MD5RGET( 0), 0xEAA127FAU, 11);
		MD5R3A(c, d, a, b, MD5RGET( 3), 0xD4EF3085U, 16);
		MD5R3B(b, c, d, a, MD5RGET( 6), 0x04881D05U, 23);
		MD5R3A(a, b, c, d, MD5RGET( 9), 0xD9D4D039U,  4);
		MD5R3B(d, a, b, c, MD5RGET(12), 0xE6DB99E5U, 11);
		MD5R3A(c, d, a, b, MD5RGET(15), 0x1FA27CF8U, 16);
		MD5R3B(b, c, d, a, MD5RGET( 2), 0xC4AC5665U, 23);

		MD5R4 (a, b, c, d, MD5RGET( 0), 0xF4292244U,  6);
		MD5R4 (d, a, b, c, MD5RGET( 7), 0x432AFF97U, 10);
		MD5R4 (c, d, a, b, MD5RGET(14), 0xAB9423A7U, 15);
		MD5R4 (b, c, d, a, MD5RGET( 5), 0xFC93A039U, 21);
		MD5R4 (a, b, c, d, MD5RGET(12), 0x655B59C3U,  6);
		MD5R4 (d, a, b, c, MD5RGET( 3), 0x8F0CCC92U, 10);
		MD5R4 (c, d, a, b, MD5RGET(10), 0xFFEFF47DU, 15);
		MD5R4 (b, c, d, a, MD5RGET( 1), 0x85845DD1U, 21);
		MD5R4 (a, b, c, d, MD5RGET( 8), 0x6FA87E4FU,  6);
		MD5R4 (d, a, b, c, MD5RGET(15), 0xFE2CE6E0U, 10);
		MD5R4 (c, d, a, b, MD5RGET( 6), 0xA3014314U, 15);
		MD5R4 (b, c, d, a, MD5RGET(13), 0x4E0811A1U, 21);
		MD5R4 (a, b, c, d, MD5RGET( 4), 0xF7537E82U,  6);
		MD5R4 (d, a, b, c, MD5RGET(11), 0xBD3AF235U, 10);
		MD5R4 (c, d, a, b, MD5RGET( 2), 0x2AD7D2BBU, 15);
		MD5R4 (b, c, d, a, MD5RGET( 9), 0xEB86D391U, 21);

		a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} while (size -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	return ptr;
}


sb_bool_t sb_crypto_md5_init(sb_crypto_md5_ctx_t *ctx) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set(SB_ERROR_NULL_PTR);
		return sb_false;
	}

	sb_crypto_md5_clear(ctx);

	ctx->a = 0x67452301U;
	ctx->b = 0xEFCDAB89U;
	ctx->c = 0x98BADCFEU;
	ctx->d = 0x10325476U;

	return sb_true;
}


sb_bool_t sb_crypto_md5_clear(sb_crypto_md5_ctx_t *ctx) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set(SB_ERROR_NULL_PTR);
		return sb_false;
	}

	sb_memset(ctx, 0, sizeof(*ctx));

	return sb_true;
}


sb_bool_t sb_crypto_md5_update(sb_crypto_md5_ctx_t *ctx, void *in, sb_size_t size) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!in) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	uint32_t saved_lo;
	sb_size_t used, available;

	saved_lo = ctx->lo;
	if ((ctx->lo = ((saved_lo + size) & 0x1FFFFFFF)) < saved_lo) {
		++ctx->hi;
	}
	ctx->hi += (uint32_t)(size >> 29);

	used = (saved_lo & 0x3F);

	uint8_t *in8 = in;

	if (used) {
		available = (64 - used);

		if (size < available) {
			sb_memcpy(&ctx->buffer[used], in8, size);
			return sb_true;
		}

		sb_memcpy(&ctx->buffer[used], in8, available);
		in8 += available;
		size -= available;
		sb_crypto_md5_internal_update(ctx, ctx->buffer, 64);
	}

	if (size >= 64) {
		in8 = sb_crypto_md5_internal_update(ctx, in8, size & ~(sb_size_t)0x3F);
		size &= 0x3F;
	}

	sb_memcpy(ctx->buffer, in8, size);

	return sb_true;
}


sb_bool_t sb_crypto_md5_finish(sb_crypto_md5_ctx_t *ctx, uint8_t out[SB_CRYPTO_HASHING_MD5_DIGEST_SIZE]) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!out) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	sb_size_t used, available;

	used = ctx->lo & 0x3F;

	ctx->buffer[used++] = 0x80;

	available = 64 - used;

	if (available < 8) {
		sb_memset(&ctx->buffer[used], 0, available);
		sb_crypto_md5_internal_update(ctx, ctx->buffer, 64);
		used = 0;
		available = 64;
	}

	sb_memset(&ctx->buffer[used], 0, available - 8);

	ctx->lo <<= 3;
	*((uint32_t*)(&ctx->buffer[56])) = SB_LE32(ctx->lo); // XXX: strict-aliasing
	*((uint32_t*)(&ctx->buffer[60])) = SB_LE32(ctx->hi); // XXX: strict-aliasing

	sb_crypto_md5_internal_update(ctx, ctx->buffer, 64);

	uint32_t *out32 = (uint32_t*)out;
	out32[0] = SB_LE32(ctx->a);
	out32[1] = SB_LE32(ctx->b);
	out32[2] = SB_LE32(ctx->c);
	out32[3] = SB_LE32(ctx->d);

	sb_memset(ctx, 0, sizeof(*ctx));

	return sb_true;
}


sb_bool_t sb_crypto_md5(uint8_t out[SB_CRYPTO_HASHING_MD5_DIGEST_SIZE], void *in, sb_size_t size) {
	sb_error_reset();

	sb_bool_t success = sb_true;
	sb_crypto_md5_ctx_t ctx;
	if (!sb_crypto_md5_init(&ctx)) {
		sb_error_set(SB_ERROR_INITIALIZATION);
		return sb_false;
	}
	if (!sb_crypto_md5_update(&ctx, in, size)) {
		sb_error_set_ex(SB_ERROR_FUNCTIONALITY, 1);
		success = sb_false;
	} else {
		if (!sb_crypto_md5_finish(&ctx, out)) {
			sb_error_set_ex(SB_ERROR_FUNCTIONALITY, 2);
			success = sb_false;
		}
	}
	if (!sb_crypto_md5_clear(&ctx)) {
		sb_error_set(SB_ERROR_CLEANUP);
		success = sb_false;
	}
	return success;
}

#else
#	ifdef REPORT_EXCLUSION
#		pragma message("Excluded: "__FILE_LOCAL__)
#	endif
#endif
