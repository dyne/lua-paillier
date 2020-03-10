FROM dyne/devuan:beowulf

RUN apt-get update -y -q && apt-get install -y -q luarocks gcc cmake 

COPY . /lua-multiparty/

WORKDIR /lua-multiparty

RUN make check
