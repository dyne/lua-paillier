/* lua-multiparty
 *
 * Copyright (C) 2020 Dyne.org foundation
 * designed, written and maintained by Denis Roio <jaromil@dyne.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <string.h>
#include <inttypes.h>
#include <time.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <amcl.h>

#include <randombytes.h>
// easier name (csprng comes from amcl.h in milagro)
#define RNG csprng
static uint8_t random_seed[256];
static RNG *rng;

static int rng_uint8(lua_State *L) {
	uint8_t res = RAND_byte(rng);
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}

static int rng_uint16(lua_State *L) {
	uint16_t res =
		RAND_byte(rng)
		| (uint32_t) RAND_byte(rng) << 8;
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}

static int rng_int32(lua_State *L) {
	uint32_t res =
		RAND_byte(rng)
		| (uint32_t) RAND_byte(rng) << 8
		| (uint32_t) RAND_byte(rng) << 16
		| (uint32_t) RAND_byte(rng) << 24;
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}

static int rng_int64(lua_State *L) {
	uint64_t res =
		RAND_byte(rng)
		| (uint64_t) RAND_byte(rng) << 8
		| (uint64_t) RAND_byte(rng) << 16
		| (uint64_t) RAND_byte(rng) << 24
		| (uint64_t) RAND_byte(rng) << 32
		| (uint64_t) RAND_byte(rng) << 40
		| (uint64_t) RAND_byte(rng) << 48
		| (uint64_t) RAND_byte(rng) << 56;
	lua_pushinteger(L, (lua_Integer)res);
	return(1);
}

static int mp_keygen (lua_State *L) { return 0; }
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
	rng = malloc(sizeof(RNG));
	if(!rng) return 0;
	randombytes(random_seed, 252); // last 4 bytes from time
	uint32_t ttmp = (uint32_t) time(NULL);
	random_seed[252] = (ttmp >> 24) & 0xff;
	random_seed[253] = (ttmp >> 16) & 0xff;
	random_seed[254] = (ttmp >>  8) & 0xff;
	random_seed[255] =  ttmp & 0xff;
	char tseed[256]; // RAND_seed is destructive, preserve seed here
	memcpy(tseed,random_seed,256);
	RAND_seed(rng, 256, tseed);

	luaL_register(L, "multiparty", multiparty);
	lua_pushliteral (L, "VERSION");
	lua_pushliteral (L, VERSION); 
	lua_settable (L, -3);
	return 1;
}
