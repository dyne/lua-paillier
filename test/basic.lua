-- local path = "/home/jrml/devel/lua-multiparty/src/lua-multiparty.so"
-- local f = package.loadlib(path, "luaopen_multiparty")
-- f()
pk = multiparty.keygen()
print("Keygen public key length: "..#pk / 2)

pt = "b353df2d862f1db5380826e1028e06094be0c568dff963bed3324d2f9cec5aafcb5f057959e0843ac1f72008da23b3511f5ef672370726e70d95981579eb7a9c8391e14536623dee6016dfb2870f1cf46cf14239d76296f6c9a59605ff84b8026471089d8b17232b78413dcb0a4116c6009348bbabe4ba205d350845f0c1f21787a48bf4b28ee88294c16d6c2f8c2917e4d24e2105a7014459a48d4a12d4122a3b02d36d9dbc45e109d0fb85b860609e89f116805ab23df378c07ab5e917da82280f71c6661a8c8d04b249a1a47c9f6e889862ce05fba7b6a58e5573f2b13fa563abddf6cb29cf500df07e03321f6f288c6722827fa6622f3dc93817e1c96883"
print("plain text length: "..#pt / 2)

ct = multiparty.encrypt(pk, pt)
print("cipher text length: "..#ct / 2)

