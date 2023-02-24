package = "paillier"
version = "scm-1"
source = {
   url = "https://github.com/dyne/lua-paillier",
}
description = {
   summary = "Homomorphic Computation based on Paillier Crypto",
   detailed = [[
Portable cryptographic module to execute homomorphic computationsbased on the Paillier crypto-scheme. Works only on 64bit systems.
]],
   license = "MIT",
   homepage = "https://github.com/dyne/lua-paillier",
}
supported_platforms = {
   "unix",
}
dependencies = {
   "lua ~> 5.1",
}
build = {
   type = "make",
   build_variables = {
      LUA_CFLAGS="$(CFLAGS)",
      LUA_LDFLAGS="$(LIBFLAG)",
      LUA_LIBDIR="$(LUA_LIBDIR)",
      LUA_BINDIR="$(LUA_BINDIR)",
      LUA_INCDIR="$(LUA_INCDIR)",
      LUA="$(LUA)",
   },
   install_variables = {
      INST_PREFIX="$(PREFIX)",
      INST_BINDIR="$(BINDIR)",
      INST_LIBDIR="$(LIBDIR)",
      INST_LUADIR="$(LUADIR)",
      INST_CONFDIR="$(CONFDIR)",
   }
}
