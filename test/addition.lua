pk, sp, sq = multiparty.keygen()
left = 1024
right = 256
result = left + right
cl = multiparty.encrypt(pk, left)
cr = multiparty.encrypt(pk, right)
cres = multiparty.add(pk, cl, cr)
print(result)
plain = multiparty.decrypt(sp, sq, cres)
print(plain)
print("TODO: octet to integer")
print("TODO: 256 and 512 byte random")
