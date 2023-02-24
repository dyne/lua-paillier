/* lua-paillier
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

#include <inttypes.h>
#include <time.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <amcl.h>

#include <randombytes.h>
// easier name (csprng comes from amcl.h in milagro)
static uint8_t random_seed[256];
csprng rng;

void prng_init() {
	randombytes(random_seed, 252); // last 4 bytes from time
	uint32_t ttmp = (uint32_t) time(NULL);
	random_seed[252] = (ttmp >> 24) & 0xff;
	random_seed[253] = (ttmp >> 16) & 0xff;
	random_seed[254] = (ttmp >>  8) & 0xff;
	random_seed[255] =  ttmp & 0xff;
	char tseed[256]; // RAND_seed is destructive, preserve seed here
	memcpy(tseed,random_seed,256);
	RAND_seed(&rng, 256, tseed);
}
int rng_uint8(lua_State *L) {
	uint8_t res = RAND_byte(&rng);
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}

int rng_uint16(lua_State *L) {
	uint16_t res =
		RAND_byte(&rng)
		| (uint32_t) RAND_byte(&rng) << 8;
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}

int rng_int32(lua_State *L) {
	uint32_t res =
		RAND_byte(&rng)
		| (uint32_t) RAND_byte(&rng) << 8
		| (uint32_t) RAND_byte(&rng) << 16
		| (uint32_t) RAND_byte(&rng) << 24;
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}

int rng_int64(lua_State *L) {
	uint64_t res =
		RAND_byte(&rng)
		| (uint64_t) RAND_byte(&rng) << 8
		| (uint64_t) RAND_byte(&rng) << 16
		| (uint64_t) RAND_byte(&rng) << 24
		| (uint64_t) RAND_byte(&rng) << 32
		| (uint64_t) RAND_byte(&rng) << 40
		| (uint64_t) RAND_byte(&rng) << 48
		| (uint64_t) RAND_byte(&rng) << 56;
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}
