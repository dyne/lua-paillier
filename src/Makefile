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

SOURCES := multiparty.o randombytes.o encoding.o prng.o

all: ${SOURCES}
	${CC} ${CFLAGS} ${SOURCES} -o multiparty.so ${LDFLAGS} ${LDADD}

check:
	lua${LUAVER} -l multiparty ../test/basic.lua
	lua${LUAVER} -l multiparty ../test/add.lua

gdb:
	gdb --args lua${LUAVER} -l multiparty ../test/basic.lua

clean:
	rm -f *.o
	rm -f *.so

.c.o:
	${CC} -I. ${CFLAGS} -c $< -o $@ -DVERSION=\"${VERSION}\"
