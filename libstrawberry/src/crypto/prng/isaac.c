#include "../../core/identid.h"
IDENTID("isaac.c", "0.1", "4", "2016-07-29");

#include "isaac.h"


#define SB_CRYPTO_PRNG_ISAAC_MIX(a, b, c, d, e, f, g, h) {								\
	a ^= ( b               << 11);	d += a; b += c;										\
	b ^= ((c & 0xFFFFFFFF) >>  2);	e += b; c += d;										\
	c ^= ( d               <<  8);	f += c; d += e;										\
	d ^= ((e & 0xFFFFFFFF) >> 16);	g += d; e += f;										\
	e ^= ( f               << 10);	h += e; f += g;										\
	f ^= ((g & 0xFFFFFFFF) >>  4);	a += f; g += h;										\
	g ^= ( h               <<  8);	b += g; h += a;										\
	h ^= ((a & 0xFFFFFFFF) >>  9);	c += h; a += b;										\
}

#define SB_CRYPTO_PRNG_ISAAC_ROUND_STEP(mix, a, b, mm, m, m2, r, x)						{ x = *m; a = ((a^(mix)) + *(m2++)); *(m++) = y = (((mm)[(x >> 2) & (SB_CRYPTO_PRNG_ISAAC_SIZE - 1)]) + a + b); *(r++) = b = (((mm)[((y >> (SB_CRYPTO_PRNG_ISAAC_SHIFT)) >> 2) & (SB_CRYPTO_PRNG_ISAAC_SIZE - 1)]) + x) & 0xFFFFFFFF; }
#define SB_CRYPTO_PRNG_ISAAC_ROUND(a, b, mm, m, m2, r, x) {								\
	SB_CRYPTO_PRNG_ISAAC_ROUND_STEP(( a               << 13), a, b, mm, m, m2, r, x);	\
	SB_CRYPTO_PRNG_ISAAC_ROUND_STEP(((a & 0xFFFFFFFF) >>  6), a, b, mm, m, m2, r, x);	\
	SB_CRYPTO_PRNG_ISAAC_ROUND_STEP(( a               <<  2), a, b, mm, m, m2, r, x);	\
	SB_CRYPTO_PRNG_ISAAC_ROUND_STEP(((a & 0xFFFFFFFF) >> 16), a, b, mm, m, m2, r, x);	\
}

void sb_crypto_prng_isaac_init(sb_crypto_prng_isaac_ctx_t *ctx, sb_bool_t preseeded) {
	register uint_fast32_t i;

	register sb_crypto_prng_isaac_int_t a, b, c, d, e, f, g, h;
	sb_crypto_prng_isaac_int_t *m, *r;

	ctx->a = ctx->b = ctx->c = 0;

	m = ctx->mem;
	r = ctx->rsl;

	a = b = c = d = e = f = g = h = 0x9E3779B9;

	for (i = 4; i--;) {
		SB_CRYPTO_PRNG_ISAAC_MIX(a, b, c, d, e, f, g, h);
	}

	if (preseeded) {
		for (i = 0; i < SB_CRYPTO_PRNG_ISAAC_SIZE; i += 8)  {
			a += r[i    ];                     b += r[i + 1];
			c += r[i + 2];                     d += r[i + 3];
			e += r[i + 4];                     f += r[i + 5];
			g += r[i + 6];                     h += r[i + 7];
			SB_CRYPTO_PRNG_ISAAC_MIX(a, b, c, d, e, f, g, h);
			m[i    ] = a ;                     m[i + 1] = b ;
			m[i + 2] = c ;                     m[i + 3] = d ;
			m[i + 4] = e ;                     m[i + 5] = f ;
			m[i + 6] = g ;                     m[i + 7] = h ;
		}
		for (i = 0; i < SB_CRYPTO_PRNG_ISAAC_SIZE; i += 8)  {
			a += m[i    ];                     b += m[i + 1];
			c += m[i + 2];                     d += m[i + 3];
			e += m[i + 4];                     f += m[i + 5];
			g += m[i + 6];                     h += m[i + 7];
			SB_CRYPTO_PRNG_ISAAC_MIX(a, b, c, d, e, f, g, h);
			m[i    ] = a ;                     m[i + 1] = b ;
			m[i + 2] = c ;                     m[i + 3] = d ;
			m[i + 4] = e ;                     m[i + 5] = f ;
			m[i + 6] = g ;                     m[i + 7] = h ;
		}
	} else {
		for (i = 0; i < SB_CRYPTO_PRNG_ISAAC_SIZE; i += 8)  {
			SB_CRYPTO_PRNG_ISAAC_MIX(a, b, c, d, e, f, g, h);
			m[i    ] = a ;                     m[i + 1] = b ;
			m[i + 2] = c ;                     m[i + 3] = d ;
			m[i + 4] = e ;                     m[i + 5] = f ;
			m[i + 6] = g ;                     m[i + 7] = h ;
		}
	}

	sb_crypto_prng_isaac_update(ctx);
	ctx->cnt = SB_CRYPTO_PRNG_ISAAC_SIZE;
}

void sb_crypto_prng_isaac_update(sb_crypto_prng_isaac_ctx_t *ctx) {
	sb_crypto_prng_isaac_int_t
		x, y, *m,
		*m2, *mend;

	sb_crypto_prng_isaac_int_t
		a = ctx->a,
		b = ctx->b + (++ctx->c),
		*mm = ctx->mem,
		*r = ctx->rsl;

	for (m = mm, mend = m2 = m + (SB_CRYPTO_PRNG_ISAAC_SIZE / 2); m < mend;) {
		SB_CRYPTO_PRNG_ISAAC_ROUND(a, b, mm, m, m2, r, x);
	}

	for (m2 = mm; m2 < mend;) {
		SB_CRYPTO_PRNG_ISAAC_ROUND(a, b, mm, m, m2, r, x);
	}

	ctx->b = b; ctx->a = a;
}

sb_crypto_prng_isaac_int_t sb_crypto_prng_isaac(sb_crypto_prng_isaac_ctx_t *ctx) {
	if (!ctx->cnt--) {
		sb_crypto_prng_isaac_update(ctx);
		ctx->cnt = (SB_CRYPTO_PRNG_ISAAC_SIZE - 1);
	}
	return ctx->rsl[ctx->cnt];
}