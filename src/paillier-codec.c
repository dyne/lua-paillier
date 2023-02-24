/* lua-multiparty
 *
 * Copyright (C) 2020 Dyne.org foundation
 * designed, written and maintained by Denis Roio <jaromil@dyne.org>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Enterprise level support contracts are available upon request, as
 * well customisations and license exceptions: you are welcome to get
 * in touch with us at https://zenroom.org
 *
 */


#include <amcl.h>
#include <paillier.h>

#include <xxxutils.h>
#include <encoding.h>

// sizeof(big 1024_58): 144
// sizeof(big 512_60): 72
// hflen 2048(4)
// fflen 4096(4)
static const uint8_t SKCHUNK = sizeof(BIG_1024_58);
static const uint16_t SKSIZE = SKCHUNK*HFLEN_2048;
static const uint16_t TOTSIZE = SKSIZE*4;

void PAILLIER_SK_toOctet(octet *P, octet *Q, PAILLIER_private_key *PRIV) {
	SAFE(P); SAFE(Q);
	FF_2048_toOctet(P, PRIV->p, HFLEN_2048);
	FF_2048_toOctet(Q, PRIV->q, HFLEN_2048);
}

void PAILLIER_SK_fromOctet(PAILLIER_private_key *PRIV, octet *P, octet *Q) {
	SAFE(P); SAFE(Q);
	FF_2048_fromOctet(PRIV->p, P, HFLEN_2048);
	FF_2048_zero(PRIV->p2, FFLEN_2048);
	FF_2048_sqr(PRIV->p2, PRIV->p, HFLEN_2048);
	FF_2048_norm(PRIV->p2, FFLEN_2048);
	FF_2048_zero(PRIV->invp, FFLEN_2048);
	FF_2048_invmod2m(PRIV->invp, PRIV->p, HFLEN_2048);

	FF_2048_fromOctet(PRIV->q, Q, HFLEN_2048);
	FF_2048_zero(PRIV->q2, FFLEN_2048);
	FF_2048_sqr(PRIV->q2, PRIV->q, HFLEN_2048);
	FF_2048_norm(PRIV->q2, FFLEN_2048);
	FF_2048_zero(PRIV->invq, FFLEN_2048);
	FF_2048_invmod2m(PRIV->invq, PRIV->q, HFLEN_2048);

	// lp = p-1, lq = q-1
	FF_2048_copy(PRIV->lp, PRIV->p, HFLEN_2048);
	FF_2048_copy(PRIV->lq, PRIV->q, HFLEN_2048);
	FF_2048_dec(PRIV->lp, 1, HFLEN_2048);
	FF_2048_dec(PRIV->lq, 1, HFLEN_2048);

	// Workspace for CRT precomputations
	BIG_1024_58 ff[FFLEN_2048];
	BIG_1024_58 dff[2*FFLEN_2048];
	BIG_1024_58 g[FFLEN_2048]; // pubkey

	// g = n + 1
	FF_2048_mul(g, PRIV->p, PRIV->q, HFLEN_2048);
	FF_2048_inc(g, 1, FFLEN_2048);

	// (((g^(p-1) mod p^2) - 1) / p)^(-1) mod p for dec/enc with CRT
	FF_2048_skpow(ff, g, PRIV->lp, PRIV->p2, FFLEN_2048, HFLEN_2048);
	FF_2048_dec(ff, 1, FFLEN_2048);
	FF_2048_mul(dff, ff, PRIV->invp, FFLEN_2048);
	FF_2048_invmodp(PRIV->mp, dff, PRIV->p, HFLEN_2048);

	// (((g^(q-1) mod q^2) - 1) / q)^(-1) mod q for dec/enc with CRT
	FF_2048_skpow(ff, g, PRIV->lq, PRIV->q2, FFLEN_2048, HFLEN_2048);
	FF_2048_dec(ff, 1, FFLEN_2048);
	FF_2048_mul(dff, ff, PRIV->invq, FFLEN_2048);
	FF_2048_invmodp(PRIV->mq, dff, PRIV->q, HFLEN_2048);
	
}

