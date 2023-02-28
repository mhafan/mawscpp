FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y g++ git make libcurl4-openssl-dev

WORKDIR /src
RUN git clone https://github.com/mhafan/mawscpp.git 
WORKDIR /src/mawscpp
RUN make install
