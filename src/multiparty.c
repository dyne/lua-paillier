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

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int keygen (lua_State *L) { return 0; }
static int add (lua_State *L) { return 0; }
static int sub (lua_State *L) { return 0; }

static const struct luaL_Reg multiparty [] = {
	// mockup
	{"keygen", keygen },
	{"add", add },
	{"sub", sub },
	{NULL, NULL}
};

LUALIB_API int luaopen_multiparty (lua_State *L){
	luaL_register(L, "multiparty", multiparty);
	lua_pushliteral (L, "VERSION");
	lua_pushliteral (L, VERSION); 
	lua_settable (L, -3);
	return 1;
}
