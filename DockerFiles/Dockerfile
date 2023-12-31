FROM debian:sid

RUN apt update

#so that we can actually build it
RUN apt install -y cmake

RUN apt install -y clang-16

ENV CXX=clang-16

#dependencies
RUN apt install -y xorg-dev