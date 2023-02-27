FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y g++ git make libcurl4-openssl-dev

WORKDIR /
RUN git clone https://github.com/mhafan/mawscpp.git
WORKDIR /mawscpp
RUN make all; make install

WORKDIR /mawscpp/testing
RUN make all