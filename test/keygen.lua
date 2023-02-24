-- local path = "/home/jrml/devel/lua-multiparty/src/lua-multiparty.so"
-- local f = package.loadlib(path, "luaopen_multiparty")
-- f()
pk, sp, sq = multiparty.keygen()
print("KEY GENERATION")
print("Keygen public key length: "..#pk / 2)
print("Keygen secret key length: "..(#sp / 2).." + "..(#sq / 2))
-- print(sp)
-- print(sq)
