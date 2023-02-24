# Lua Paillier

[![Run Tests](https://github.com/dyne/lua-paillier/actions/workflows/test.yml/badge.svg)](https://github.com/dyne/lua-paillier/actions/workflows/test.yml)

Lua Paillier is a portable cryptographic module to execute homomorphic computations based on the [Paillier cryptosystem](https://en.wikipedia.org/wiki/Paillier_cryptosystem).

This Lua module works only on 64-bit systems.

[![software by Dyne.org](https://files.dyne.org/software_by_dyne.png)](http://www.dyne.org)

# Usage

Build:

```
luarocks build paillier-scm-1.rockspec
```

Test:
```
make check
```

Launch:
```
lua -l paillier
```

## Use:
Homomorphic Properties of Paillier Cryptosystem:
* The product of two ciphers decrypts to the sum of the plain text
* The product of a cipher with a non-cipher raising g will decrypt to their sum
* A Cipher raised to a non-cipher decrypts to their product
* Any cipher raised to an integer k will decrypt to the product of the deciphered and k

Addition proof:
```
pk, sp, sq = paillier.keygen()
left = 1024
right = 256
result = left + right
cl = paillier.encrypt(pk, left)
cr = paillier.encrypt(pk, right)
cres = paillier.add(pk, cl, cr)
plain = paillier.decrypt(sp, sq, cres)
```
The hex values of `plain` and `result` are the same.

Multiplication proof:
```
pk, sp, sq = paillier.keygen()
left = 1000
right = 255 -- ff
result = left * right
cl = paillier.encrypt(pk, left)
cres = paillier.mult(pk, cl, addzero('ff',256*2)) -- zero padded hex
plain = paillier.decrypt(sp, sq, cres)
```
Again the hex values of `plain` and `result` are the same.


# Credits

Lua Paillier is Copyright (C) 2020-2023 by the Dyne.org foundation

Designed, written and maintained by Denis "Jaromil" Roio.

More contact information is on [Zenroom.org](https://zenroom.org)

This software Includes the cryptographic library Milagro released
under the Apache License, Version 2.0
- Copyright (C) 2016 MIRACL UK Ltd
- Copyright (C) 2019 The Apache Software Foundation

Special thanks to Thomas Fuerstner.

## Licensing

Lua Paillier is free software distributed under the terms of the
MIT license reproduced here. Lua Multi Party may be used for any
purpose, including commercial purposes, at absolutely no cost.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Support

Enterprise level support contracts are available upon request, as
well customisations and license exceptions: you are welcome to get
in touch with us at https://zenroom.org
