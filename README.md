# Lua Multi Party

This software is less entertaining than it sounds from the name `;^)`

Lua Multi Party is a portable cryptographic module to execute [secure multi-party computation](https://en.wikipedia.org/wiki/Secure_multi-party_computation) within [distributed computing](https://en.wikipedia.org/wiki/Distributed_computing) environments, based on the [Paillier cryptosystem](https://en.wikipedia.org/wiki/Paillier_cryptosystem).

This Lua module works only on 64-bit systems.

[![software by Dyne.org](https://files.dyne.org/software_by_dyne.png)](http://www.dyne.org)

# Usage

Build:

```
luarocks build multiparty-scm-1.rockspec
```

Test:
```
make check
```

Launch:
```
lua -l multiparty
```

Use:
```
pk = multiparty.keygen()
```

## WORK IN PROGRESS

# Credits

Lua Multi Party is Copyright (C) 2020 by the Dyne.org foundation

Designed, written and maintained by Denis "Jaromil" Roio.

More contact information is on [Zenroom.org](https://zenroom.org)

Special thanks to Thomas Fuerstner for the encouragement.

Lua Multi Party is free software distributed under the terms of the
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
