-- local path = "/home/jrml/devel/lua-paillier/src/lua-paillier.so"
-- local f = package.loadlib(path, "luaopen_paillier")
-- f()
pk, sp, sq = paillier.keygen()
print("KEY GENERATION")
print("Keygen public key length: "..#pk / 2)
print("Keygen secret key length: "..(#sp / 2).." + "..(#sq / 2))
-- print(sp)
-- print(sq)
