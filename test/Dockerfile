FROM dyne/devuan:beowulf

RUN apt-get update -y -q && apt-get install -y -q luarocks gcc cmake 

COPY . /lua-paillier/

WORKDIR /lua-paillier

RUN make check
