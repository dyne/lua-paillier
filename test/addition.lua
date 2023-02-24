print("TEST HOMOMORPHIC ADDITION PROPERTY")

function trim(hex)
   while true do
      if hex:sub(1,1) == '0' then
         hex = hex:sub(2)
      else
         break
      end
   end
   return hex
end

pk, sp, sq = paillier.keygen()
left = 1024
right = 256
result = left + right
cl = paillier.encrypt(pk, left)
cr = paillier.encrypt(pk, right)
cres = paillier.add(pk, cl, cr)
plain = paillier.decrypt(sp, sq, cres)

-- to compare: convert all to hex and trim leading zeros
assert( trim(plain) == string.format("%x",result) )

print("-- SUCCESS")
