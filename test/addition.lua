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

pk, sp, sq = multiparty.keygen()
left = 1024
right = 256
result = left + right
cl = multiparty.encrypt(pk, left)
cr = multiparty.encrypt(pk, right)
cres = multiparty.add(pk, cl, cr)
plain = multiparty.decrypt(sp, sq, cres)

-- to compare: convert all to hex and trim leading zeros
assert( trim(plain) == string.format("%x",result) )

print("-- SUCCESS")
