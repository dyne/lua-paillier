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

#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <amcl.h>
#include <paillier.h>

#include <xxxutils.h>
#include <prng.h>
#include <encoding.h>

octet *o_alloc(const uint32_t size) {
	octet *o = malloc(sizeof(octet));
	if(!o) return(NULL);
	o->val = malloc(size + 0x0f);
	if(!o->val) { free(o); return(NULL); }
	o->len = 0;
	o->max = size;
	xxx("octet alloc: %p (%u)",o, size);
	return(o);
}

// here most internal type conversions happen
octet* o_arg(lua_State *L, int n) {
	octet *o;
	if(strncmp("string",luaL_typename(L,n),6)==0) {
		size_t len; const char *str;
		str = luaL_optlstring(L,n,NULL,&len);
		if(!str || !len) {
			luaL_error(L, "cannot get octet arg #%u", n);
			return(NULL); }
		if(!is_hex(str)) {
			luaL_error(L, "invalid hex octet arg #%u", n);
			return(NULL); }
		// fallback to a string
		o = o_alloc(hex2oct_len(str)); // new
		hex2oct(o, str);
	} else {
		luaL_error(L, "no string type octet arg #%u", n);
		return(NULL);
	}
	xxx("octet arg: %i %p (%u)",n,o,o->len);

	return(o);
}

octet *o_dup(octet *o) {
	SAFE(o);
	octet *n = o_alloc(o->len+1);
	OCT_copy(n,o);
	xxx("octet dup: %p (%u)",o,o->len);
	return(n);
}

void o_free(octet *o) {
	SAFE(o);
	if(o) {
		if(o->val) free(o->val);
		xxx("octet free: %p (%u/%u)",o, o->len, o->max);
		free(o);
	}
}

// TODO: paillier.h
extern void PAILLIER_SK_toOctet(octet *P, octet *Q, PAILLIER_private_key *PRIV);
extern void PAILLIER_SK_fromOctet(PAILLIER_private_key *PRIV, octet *P, octet *Q);

#define pushoctet(x) \
	{ char *s; \
	  s = malloc( oct2hex_len(x) ); SAFE(s); \
	  oct2hex(s, x); lua_pushstring(L,s); free(s); }


static int mp_keygen (lua_State *L) {
	char *s;
	octet *pk;
	PAILLIER_public_key pub;
	PAILLIER_private_key priv;
	PAILLIER_KEY_PAIR(&rng, NULL, NULL, &pub, &priv);

	pk = o_alloc(4096); SAFE(pk);
	PAILLIER_PK_toOctet(pk, &pub);
	pushoctet(pk); o_free(pk);

	octet *P, *Q; // secret key
	P = o_alloc(HFLEN_2048*MODBYTES_1024_58);
	Q = o_alloc(HFLEN_2048*MODBYTES_1024_58);
	PAILLIER_SK_toOctet(P, Q, &priv);
	pushoctet(P); o_free(P);
	pushoctet(Q); o_free(Q);

	return 3;
}

static int mp_encrypt (lua_State *L) {
	// arg 1: public key
	octet *pk = o_arg(L, 1); SAFE(pk);
	if(pk->len != 256) {
		xxx("encrypt pk arg len != 256 (%u)",pk->len);
		return(0); }
	PAILLIER_public_key pub;
	PAILLIER_PK_fromOctet(&pub, pk);
	// arg 2: number
	lua_Number n = lua_tointeger(L,2);
	BIG_1024_58 pt[FFLEN_2048];
	octet *plain;
	xxx("encrypting number: %u",(int)n);
	if(n) {
		plain = o_alloc(FS_4096); SAFE(plain);
		FF_2048_init(pt, (uint32_t)n ,FFLEN_2048);
		FF_2048_toOctet(plain, pt, FFLEN_2048);
	} else return 0;
	// octet *plain = o_arg(L, 2); SAFE(plain);
	octet *ct = o_alloc(FS_4096); SAFE(ct);
	PAILLIER_ENCRYPT(&rng,  &pub, plain, ct, NULL);
	pushoctet(ct);
	o_free(ct);
	o_free(plain);
	return 1;
}

static int mp_decrypt (lua_State *L) {
	octet *P = o_arg(L, 1); SAFE(P);
	octet *Q = o_arg(L, 2); SAFE(Q);
	octet *ct = o_arg(L, 3); SAFE(ct);
	PAILLIER_private_key priv;
	PAILLIER_SK_fromOctet(&priv, P, Q);
	octet *pt = o_alloc(256); SAFE(pt);
	PAILLIER_DECRYPT(&priv, ct, pt);
	pushoctet(pt); o_free(pt);
	return 1;
}

static int mp_add (lua_State *L) {
	octet *pk = o_arg(L, 1); SAFE(pk);
	if(pk->len != 512 && pk->len != 256) {
		xxx("encrypt pk arg len != 512 (%u)",pk->len);
		return(0); }
	octet *ct1 = o_arg(L, 2); SAFE(ct1);
	octet *ct2 = o_arg(L, 3); SAFE(ct2);
	octet *res = o_alloc(512); SAFE(res);
	PAILLIER_public_key pub;
	PAILLIER_PK_fromOctet(&pub, pk);
	PAILLIER_ADD(&pub, ct1, ct2, res);
	pushoctet(res);
	o_free(res);
	return 1;
}

static int mp_mult (lua_State *L) {
	octet *pk = o_arg(L, 1); SAFE(pk);
	if(pk->len != 512 && pk->len != 256) {
		xxx("encrypt pk arg len != 512 (%u)",pk->len);
		return(0); }
	octet *ct1 = o_arg(L, 2); SAFE(ct1);
	octet *pt2 = o_arg(L, 3); SAFE(pt2);
	octet *res = o_alloc(512); SAFE(res);
	PAILLIER_public_key pub;
	PAILLIER_PK_fromOctet(&pub, pk);
	PAILLIER_MULT(&pub, ct1, pt2, res);
	pushoctet(res);
	o_free(res);
	return 1;
}

static const struct luaL_Reg multiparty [] = {

	{"keygen",  mp_keygen },
	{"encrypt", mp_encrypt },
	{"decrypt", mp_decrypt },
	{"add",     mp_add },  // Homomorphic addition of plaintexts
	{"mult",    mp_mult }, // Homomorphic multipication of plaintexts
	// random
	{"random_int8",  rng_uint8  },
	{"random_int16", rng_uint16 },
	{"random_int32", rng_int32 },
	{"random_int64", rng_int64 },
	{"random8",      rng_uint8  },
	{"random16",     rng_uint16 },
	{"random32",     rng_int32 },
	{"random64",     rng_int64 },
		 
	{NULL, NULL}
};

LUALIB_API int luaopen_multiparty (lua_State *L){

	// PRNG: initialise the pseudo-random generator
	prng_init();
	xxx("SIZES OF PRIMITIVES");
	xxx("modbytes 1024_58: %u", MODBYTES_1024_58);
	xxx("sizeof(big 1024_58): %u", sizeof(BIG_1024_58));
	xxx("sizeof(big 512_60): %u", sizeof(BIG_512_60));
	xxx("hflen 2048(%u)", sizeof(HFLEN_2048));
	xxx("fflen 4096(%u)", sizeof(FFLEN_4096));
	luaL_register(L, "multiparty", multiparty);
	lua_pushliteral (L, "VERSION");
	lua_pushliteral (L, VERSION); 
	lua_settable (L, -3);
	return 1;
}

