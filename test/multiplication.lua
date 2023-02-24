print("TEST HOMOMORPHIC MULTIPLICATION PROPERTY")

local function trim(hex)
   while true do
      if hex:sub(1,1) == '0' then
         hex = hex:sub(2)
      else
         break
      end
   end
   return hex
end
local function addzero(hex, length)
   hex = string.rep('0', length-#hex)..hex
   return hex
end

pk, sp, sq = multiparty.keygen()
left = 1000
right = 255 -- ff
result = left * right
cl = multiparty.encrypt(pk, left)
cres = multiparty.mult(pk, cl, addzero('ff',256*2))
plain = multiparty.decrypt(sp, sq, cres)

-- to compare: convert all to hex and trim leading zeros
assert( trim(plain) == string.format("%x",result) )

print("-- SUCCESS")
