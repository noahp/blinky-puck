FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get -y install \
    binutils-arm-none-eabi \
    build-essential \
    gcc-arm-none-eabi \
    gcc-arm-none-eabi-source \
    git \
    libnewlib-arm-none-eabi \
    libnewlib-nano-arm-none-eabi \
    lld \
    llvm

# Unused for now
# clang \
# clang-format \

# get user id from build arg, so we can have read/write access to directories
# mounted inside the container. only the UID is necessary, UNAME just for
# cosmetics
ARG UID=1010
ARG UNAME=builder

RUN useradd --uid $UID --create-home --user-group ${UNAME} && \
    echo "${UNAME}:${UNAME}" | chpasswd && adduser ${UNAME} sudo

USER ${UNAME}

ENV LANG=C.UTF-8 LC_ALL=C.UTF-8

ENV PATH /home/${UNAME}/.local/bin:$PATH

WORKDIR /mnt/workspace
