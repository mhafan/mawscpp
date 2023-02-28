FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y g++ git make libcurl4-openssl-dev

WORKDIR /mawsgo
RUN git clone https://github.com/mhafan/mawscpp.git && make install
