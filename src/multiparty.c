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

#include <prng.h>
#include <encoding.h>

void xxx(const char *format, ...) {
#ifdef DEBUG
	va_list arg;
	va_start(arg, format);
	vfprintf(stderr, format, arg);
	fprintf(stderr,"\n");
	va_end(arg);
#endif
}

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
	xxx("octet dup: %p (%u)",o,o->len);
	octet *n = o_alloc(o->len+1);
	OCT_copy(n,o);
	return(n);
}

void o_free(octet *o) {
	xxx("octet free: %p (%u)",o, o->len);
	if(o) {
		if(o->val) free(o->val);
		free(o);
	}
}

static int mp_keygen (lua_State *L) {
	octet *out;
	PAILLIER_public_key pub;
	PAILLIER_private_key priv;
	PAILLIER_KEY_PAIR(&rng, NULL, NULL, &pub, &priv);
	out = o_alloc(256);
	PAILLIER_PK_toOctet(out, &pub);
	char *s = malloc( oct2hex_len(out) );
	oct2hex(s, out);
	o_free(out);
	lua_pushstring(L,s);
	free(s); 
	return 1;
}
static int mp_encrypt (lua_State *L) { return 0; }
static int mp_decrypt (lua_State *L) { return 0; }
static int mp_add (lua_State *L) { return 0; }
static int mp_mult (lua_State *L) { return 0; }

static const struct luaL_Reg multiparty [] = {

	// mockup
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

	luaL_register(L, "multiparty", multiparty);
	lua_pushliteral (L, "VERSION");
	lua_pushliteral (L, VERSION); 
	lua_settable (L, -3);
	return 1;
}
