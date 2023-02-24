#  lua-paillier (GNU Makefile build system)
#
#  (c) Copyright 2020-2023 Dyne.org foundation
#  designed, written and maintained by Denis Roio <jaromil@dyne.org>
#
#  This program is free software: you can redistribute it and/or
#  modify it under the terms of the GNU General Public License version
#  3 as published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see
#  <http://www.gnu.org/licenses/>.

version=0.1.0
pwd := $(shell pwd)
ARCH=$(shell uname -m)
system := Linux
gcc := gcc
luaver := 5.1
ldflags := -shared
cflags := -I ${pwd}/lib/milagro-crypto-c/include

# ----------------
# milagro settings
rsa_bits := ""
ecdh_curve := ""
ecp_curve  := ""
milagro_cmake_flags := -DBUILD_SHARED_LIBS=OFF -DBUILD_PYTHON=OFF -DBUILD_DOXYGEN=OFF -DBUILD_DOCS=OFF -DBUILD_BENCHMARKS=OFF -DBUILD_EXAMPLES=OFF -DWORD_SIZE=64 -DBUILD_PAILLIER=ON -DBUILD_X509=OFF -DBUILD_WCC=OFF -DBUILD_MPIN=OFF -DAMCL_CURVE="","" -DAMCL_RSA="" -DCMAKE_SHARED_LIBRARY_LINK_FLAGS="" -DC99=1
milib := ${pwd}/lib/milagro-crypto-c/build/lib
ldadd += ${milib}/libamcl_paillier.a
ldadd += ${milib}/libamcl_core.a

# lua flags when not built with luarocks
LUA_INCDIR ?= /usr/include/lua5.1
LUA_CFLAGS ?= -fPIC -I.
LUA_LDFLAGS ?= -fPIC

all: milagro paillier

debug: cflags += -O0 -ggdb -DDEBUG
debug: milagro paillier
	CC="${gcc}" AR="${ar}" CFLAGS="${cflags} ${LUA_CFLAGS} -I${LUA_INCDIR}" \
		LDFLAGS="${ldflags} ${LUA_LDFLAGS}" LDADD="${ldadd}" \
		VERSION="${version}" \
		make -C src

paillier:
	CC="${gcc}" AR="${ar}" CFLAGS="${cflags} ${LUA_CFLAGS} -I${LUA_INCDIR}" \
		LDFLAGS="${ldflags} ${LUA_LDFLAGS}" LDADD="${ldadd}" \
		VERSION="${version}" \
		make -C src

# CC=${gcc} CFLAGS="${cflags}" AR=${ar} RANLIB=${ranlib} LD=${ld}
milagro: CC=${gcc}
milagro: LD=${ld}
milagro:
	@if ! [ -r ${milib}/libamcl_paillier.a ]; then mkdir -p ${pwd}/lib/milagro-crypto-c/build && cd ${pwd}/lib/milagro-crypto-c/build && cmake ../ -DCMAKE_C_FLAGS="${cflags}" -DCMAKE_SYSTEM_NAME="${system}" -DCMAKE_AR=/usr/bin/ar -DCMAKE_C_COMPILER=${gcc} ${milagro_cmake_flags} && make; fi

check: cflags += -O0 -ggdb -DDEBUG
check: milagro paillier
	LUAVER=${luaver} make -C src check

gdb:
	LUAVER=${luaver} make -C src gdb

milagro-clean:
	rm -rf ${pwd}/lib/milagro-crypto-c/build

install:
	install src/paillier.so ${INST_LIBDIR}/paillier.so

clean: milagro-clean
	make clean -C src
