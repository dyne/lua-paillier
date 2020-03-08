#  lua-multiparty (GNU Makefile build system)
#
#  (c) Copyright 2020 Dyne.org foundation
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

# ----------------
# milagro settings
rsa_bits := ""
ecdh_curve := ""
ecp_curve  := ""
milagro_cmake_flags := -DBUILD_SHARED_LIBS=OFF -DBUILD_PYTHON=OFF -DBUILD_DOXYGEN=OFF -DBUILD_DOCS=OFF -DBUILD_BENCHMARKS=OFF -DBUILD_EXAMPLES=OFF -DWORD_SIZE=64 -DBUILD_PAILLIER=ON -DBUILD_X509=OFF -DBUILD_WCC=OFF -DBUILD_MPIN=OFF -DAMCL_CURVE="","" -DAMCL_RSA="" -DCMAKE_SHARED_LIBRARY_LINK_FLAGS="" -DC99=1
milib := ${pwd}/lib/milagro-crypto-c/lib
ldadd += ${milib}/libamcl_paillier.a
ldadd += ${milib}/libamcl_core.a

all: milagro multiparty

# multiparty: cflags += -I/usr/include/lua${luaver} -I/usr/include/x86_64-linux-gnu/
multiparty:
	CC="${gcc}" AR="${ar}" CFLAGS="${cflags} ${LUA_CFLAGS} -I${LUA_INCDIR}" \
		LDFLAGS="${ldflags} ${LUA_LDFLAGS}" LDADD="${ldadd}" \
		VERSION="${version}" \
		make -C src

milagro:
	if ! [ -r ${pwd}/lib/milagro-crypto-c/CMakeCache.txt ]; then cd ${pwd}/lib/milagro-crypto-c && CC=${gcc} LD=${ld} cmake . -DCMAKE_C_FLAGS="${cflags}" -DCMAKE_SYSTEM_NAME="${system}" -DCMAKE_AR=/usr/bin/ar -DCMAKE_C_COMPILER=${gcc} ${milagro_cmake_flags}; fi
	if ! [ -r ${pwd}/lib/milagro-crypto-c/lib/libamcl_core.a ]; then CC=${gcc} CFLAGS="${cflags}" AR=${ar} RANLIB=${ranlib} LD=${ld} make -C ${pwd}/lib/milagro-crypto-c VERBOSE=1; fi

check:
	LUAVER=${luaver} make -C src check

milagro-clean:
	make clean -C ${pwd}/lib/milagro-crypto-c
	rm -f ${pwd}/lib/milagro-crypto-c/CMakeCache.txt
	rm -rf ${pwd}/lib/milagro-crypto-c/CMakeFiles

install:
	install src/multiparty.so ${INST_LIBDIR}/multiparty.so

clean:
	make clean -C src

