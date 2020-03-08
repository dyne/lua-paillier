-- local path = "/home/jrml/devel/lua-multiparty/src/lua-multiparty.so"
-- local f = package.loadlib(path, "luaopen_multiparty")
-- f()
base64 = require('codec').base64
pk = multiparty.keygen()
print(type(pk))
print(base64.encode(pk))

