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
**    // XXX TODO FIXME: fails test every once in a while, search and destroy.
**
*/

#define __FILE_LOCAL__						"crypto/key_exchange/diffiehellman.c"

#if !defined(SB_EXCLUDE_CRYPTO_KEX_EXCHANGE) && !defined(SB_EXCLUDE_CRYPTO_KEX_EXCHANGE_DIFFIEHELLMAN)
#ifdef HAVE_GMP


#include "./diffiehellman.h"

#include <gmp.h>

#include "../../core/error.h"
#include "../../core/memory.h"
#include "../../misc/gmp.h"

#include "../../core/poison.h"


IDENTID(__FILE_LOCAL__, "0.3", "1", "2016-10-10");


typedef struct __sb_crypto_diffiehellman_ctx {
	mpz_t g;	// generator
	mpz_t m;	// modulo
	mpz_t pr;	// private
	mpz_t pu;	// public
	mpz_t s;	// secret
	gmp_randstate_t rand;
} __sb_crypto_diffiehellman_ctx_t;


static void sb_crypto_diffiehellman_perform(mpz_t out, mpz_t x, mpz_t y, mpz_t m) {
	if (mpz_cmp_ui(y, 1UL) == 0) {
		mpz_mod(out, x, m);
		return;
	}

	sb_bool_t even = mpz_even_p(y);

	mpz_t d;
	mpz_init(d);

	if (even) {
		mpz_sub_ui(d, y, 1UL);
	}

	mpz_mdiv_ui(d, y, 2UL);

	sb_crypto_diffiehellman_perform(out, x, d, m);

	mpz_mul(d, out, out);
	mpz_mod(out, d, m);

	if (!even) {
		mpz_mul(d, out, x);
		mpz_mod(out, d, m);
	}

	mpz_clear(d);
}


sb_bool_t sb_crypto_diffiehellman_init(sb_crypto_diffiehellman_ctx_t *ctx, uint16_t bits, uint64_t seed) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	__sb_crypto_diffiehellman_ctx_t *ictx = ctx->data = sb_malloc_s(sizeof(*ictx));

	mpz_init(ictx->g);
	mpz_init(ictx->m);
	mpz_init(ictx->pr);
	mpz_init(ictx->pu);
	mpz_init(ictx->s);

	gmp_randinit_mt(ictx->rand);

	if (seed) {
		gmp_randseed_ui(ictx->rand, seed);
	}

	ctx->bits = (bits ? bits : SB_CRYPTO_DIFFIEHELLMAN_DEFAULT_BITCOUNT);

	return sb_true;
}


sb_bool_t sb_crypto_diffiehellman_clear(sb_crypto_diffiehellman_ctx_t *ctx) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!ctx->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}


	if (!SB_FLAG(ctx->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		__sb_crypto_diffiehellman_ctx_t *ictx = ctx->data;

		mpz_clear(ictx->g);
		mpz_clear(ictx->m);
		mpz_clear(ictx->pr);
		mpz_clear(ictx->pu);
		mpz_clear(ictx->s);

		gmp_randclear(ictx->rand);

		ictx = NULL;
	}

	ctx->data = sb_free(ctx->data);

	ctx->bits = 0;

	return sb_true;
}


sb_bool_t sb_crypto_diffiehellman_generate_base(sb_crypto_diffiehellman_ctx_t *ctx) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!ctx->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	if (SB_FLAG(ctx->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set(SB_ERROR_STATE_INVALID);
		return sb_false;
	}

	__sb_crypto_diffiehellman_ctx_t *ictx = ctx->data;

	mpz_urandomb(ictx->g, ictx->rand, ctx->bits);
	mpz_nextprime(ictx->g, ictx->g);

	mpz_urandomb(ictx->m, ictx->rand, ctx->bits);
	mpz_nextprime(ictx->m, ictx->m);

	if (mpz_cmp(ictx->g, ictx->m) > 0) {
		mpz_swap(ictx->g, ictx->m);
	}

	return sb_true;
}


sb_bool_t sb_crypto_diffiehellman_generate_keys(sb_crypto_diffiehellman_ctx_t *ctx) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!ctx->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	if (SB_FLAG(ctx->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set(SB_ERROR_STATE_INVALID);
		return sb_false;
	}

	__sb_crypto_diffiehellman_ctx_t *ictx = ctx->data;

	mpz_urandomb(ictx->pr, ictx->rand, ctx->bits);

	sb_crypto_diffiehellman_perform(ictx->pu, ictx->g, ictx->pr, ictx->m);

	return sb_true;
}


sb_bool_t sb_crypto_diffiehellman_generate(sb_crypto_diffiehellman_ctx_t *ctx) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set(SB_ERROR_NULL_PTR);
		return sb_false;
	}

	return (sb_crypto_diffiehellman_generate_base(ctx) && sb_crypto_diffiehellman_generate_keys(ctx));
}


sb_bool_t sb_crypto_diffiehellman_generate_secret(sb_crypto_diffiehellman_ctx_t *ctx, void *pk_bob) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!ctx->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	if (SB_FLAG(ctx->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set(SB_ERROR_STATE_INVALID);
		return sb_false;
	}

	sb_size_t size = sb_crypto_diffiehellman_port_size(ctx);
	if (size == 0 || size >= 65535) {
		sb_error_set(SB_ERROR_VALUE_INVALID);
		return sb_false;
	}

	__sb_crypto_diffiehellman_ctx_t *ictx = ctx->data;

	mpz_t pk;
	mpz_init(pk);
	sb_mpz_import_ex(pk, pk_bob, size, 1, 1, 1, 0);

	sb_crypto_diffiehellman_perform(ictx->s, pk, ictx->pr, ictx->m);

	mpz_clear(pk);

	return sb_true;
}


sb_bool_t sb_crypto_diffiehellman_finish(sb_crypto_diffiehellman_ctx_t *ctx) {
	sb_error_reset();

	if (!ctx) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!ctx->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	if (SB_FLAG(ctx->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set(SB_ERROR_STATE_INVALID);
		return sb_false;
	}

	__sb_crypto_diffiehellman_ctx_t *ictx = ctx->data;

	mpz_clear(ictx->g);
	mpz_clear(ictx->m);
	mpz_clear(ictx->pr);
	mpz_clear(ictx->pu);

	gmp_randclear(ictx->rand);

	void *ndata = sb_malloc_s(sb_crypto_diffiehellman_port_size(ctx));
	sb_crypto_diffiehellman_export_secret(ctx, ndata);
	mpz_clear(ictx->s);

	ictx = NULL;

	ctx->data = ndata;
	ctx->flags |= SB_CRYPTO_DIFFIEHELLMAN_DONE;

	return sb_true;
}


sb_bool_t sb_crypto_diffiehellman_copy_base(sb_crypto_diffiehellman_ctx_t *dst, sb_crypto_diffiehellman_ctx_t *src) {
	sb_error_reset();

	if (!dst) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!dst->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	if (SB_FLAG(dst->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set_ex(SB_ERROR_STATE_INVALID, 1);
		return sb_false;
	}

	if (!src) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 3);
		return sb_false;
	}

	if (!src->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 4);
		return sb_false;
	}

	if (SB_FLAG(src->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set_ex(SB_ERROR_STATE_INVALID, 2);
		return sb_false;
	}

	__sb_crypto_diffiehellman_ctx_t
		*idst = dst->data,
		*isrc = src->data;

	mpz_set(idst->g, isrc->g);
	mpz_set(idst->m, isrc->m);

	return sb_true;
}


sb_bool_t sb_crypto_diffiehellman_copy_keys(sb_crypto_diffiehellman_ctx_t *dst, sb_crypto_diffiehellman_ctx_t *src) {
	sb_error_reset();

	if (!dst) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);
		return sb_false;
	}

	if (!dst->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);
		return sb_false;
	}

	if (SB_FLAG(dst->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set_ex(SB_ERROR_STATE_INVALID, 1);
		return sb_false;
	}

	if (!src) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 3);
		return sb_false;
	}

	if (!src->data) {
		sb_error_set_ex(SB_ERROR_NULL_PTR, 4);
		return sb_false;
	}

	if (SB_FLAG(src->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {
		sb_error_set_ex(SB_ERROR_STATE_INVALID, 2);
		return sb_false;
	}

	__sb_crypto_diffiehellman_ctx_t
		*idst = dst->data,
		*isrc = src->data;

	mpz_set(idst->pu, isrc->pu);
	mpz_set(idst->pr, isrc->pr);

	return sb_true;
}


sb_size_t sb_crypto_diffiehellman_port_size(sb_crypto_diffiehellman_ctx_t *ctx) {
	return ((ctx && (ctx->bits)) ? ((ctx->bits) / 8) : 0);
}


#define DH_IMPORT(v, s)										\
	sb_error_reset();										\
															\
	if (!ctx) {												\
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);				\
		return sb_false;									\
	}														\
															\
	if (!ctx->data) {										\
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);				\
		return sb_false;									\
	}														\
															\
	if (SB_FLAG(ctx->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {\
		sb_error_set(SB_ERROR_STATE_INVALID);				\
		return sb_false;									\
	}														\
															\
	if (!in) {												\
		sb_error_set_ex(SB_ERROR_NULL_PTR, 3);				\
		return sb_false;									\
	}														\
															\
	sb_mpz_import_ex(v, in, s, 1, 1, 1, 0);					\
															\
	return sb_true;


sb_bool_t sb_crypto_diffiehellman_import_generator(sb_crypto_diffiehellman_ctx_t *ctx, void *in) {
	DH_IMPORT(((__sb_crypto_diffiehellman_ctx_t*)ctx->data)->g, sb_crypto_diffiehellman_port_size(ctx));
}


sb_bool_t sb_crypto_diffiehellman_import_modulo(sb_crypto_diffiehellman_ctx_t *ctx, void *in) {
	DH_IMPORT(((__sb_crypto_diffiehellman_ctx_t*)ctx->data)->m, sb_crypto_diffiehellman_port_size(ctx));
}


sb_bool_t sb_crypto_diffiehellman_import_public(sb_crypto_diffiehellman_ctx_t *ctx, void *in) {
	DH_IMPORT(((__sb_crypto_diffiehellman_ctx_t*)ctx->data)->pu, sb_crypto_diffiehellman_port_size(ctx));
}


#undef DH_IMPORT


#define DH_EXPORT(v)										\
	sb_error_reset();										\
															\
	if (!ctx) {												\
		sb_error_set_ex(SB_ERROR_NULL_PTR, 1);				\
		return sb_false;									\
	}														\
															\
	if (!ctx->data) {										\
		sb_error_set_ex(SB_ERROR_NULL_PTR, 2);				\
		return sb_false;									\
	}														\
															\
	if (SB_FLAG(ctx->flags, SB_CRYPTO_DIFFIEHELLMAN_DONE)) {\
		sb_error_set(SB_ERROR_STATE_INVALID);				\
		return sb_false;									\
	}														\
															\
	if (!out) {												\
		sb_error_set_ex(SB_ERROR_NULL_PTR, 3);				\
		return sb_false;									\
	}														\
															\
	sb_mpz_export_ex(out, v, NULL, 1, 1, 1, 0);				\
															\
	return sb_true;


sb_bool_t sb_crypto_diffiehellman_export_generator(sb_crypto_diffiehellman_ctx_t *ctx, void *out) {
	DH_EXPORT(((__sb_crypto_diffiehellman_ctx_t*)ctx->data)->g);
}


sb_bool_t sb_crypto_diffiehellman_export_modulo(sb_crypto_diffiehellman_ctx_t *ctx, void *out) {
	DH_EXPORT(((__sb_crypto_diffiehellman_ctx_t*)ctx->data)->m);
}


sb_bool_t sb_crypto_diffiehellman_export_public(sb_crypto_diffiehellman_ctx_t *ctx, void *out) {
	DH_EXPORT(((__sb_crypto_diffiehellman_ctx_t*)ctx->data)->pu);
}


sb_bool_t sb_crypto_diffiehellman_export_secret(sb_crypto_diffiehellman_ctx_t *ctx, void *out) {
	DH_EXPORT(((__sb_crypto_diffiehellman_ctx_t*)ctx->data)->s);
}


#undef DH_EXPORT


#else
#	error crypto/key_exchange/diffiehellman.c requires libgmp
#endif
#else
#	ifdef REPORT_EXCLUSION
#		pragma message("Excluded: "__FILE_LOCAL__)
#	endif
#endif
