####################
# Setup base image #
####################

# Make sure a test build succeeds before committing a bump to this
FROM ubuntu:jammy-20240227 AS base

RUN apt-get update -y \
    && apt-get install -y make libgmp3-dev libmpc-dev libmpfr-dev texinfo \
       nasm xz-utils g++ curl git

ENV TARGET=i386-elf
ENV PREFIX="/opt/cross"
ENV PATH="$PREFIX/bin:$PATH"

## binutils
FROM base AS binutils

WORKDIR /build

ARG BINUTILS_VERSION=2.42
RUN curl "https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz" | tar -xJC / \
    &&  /binutils-${BINUTILS_VERSION}/configure --target="$TARGET" --prefix="$PREFIX" \
        --with-sysroot --disable-nls --disable-werror \
    && make \
    && make install

## gcc
FROM base AS gcc

WORKDIR /build
COPY --from=binutils /opt/cross /opt/cross

ARG GCC_VERSION=13.2.0
# TODO: Add argument for setting `-j 8` parallelism?
# Note: If debugging issues with `make all-target-libgcc`, split this into multiple RUN commands
#       to ensure that Docker build cache can prevent rerunning `make all-gcc`
RUN curl "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz" | tar -xJC / \
    && /gcc-${GCC_VERSION}/configure --target="$TARGET" --prefix="$PREFIX" --disable-nls \
       --enable-languages=c,c++ --without-headers \
    && make -j 8 all-gcc \
    && make all-target-libgcc \
    && make install-gcc \
    && make install-target-libgcc

###############
# Build image #
###############

FROM base AS builder

ARG GNU_EFI_VERSION=3.0.18
RUN git clone https://git.code.sf.net/p/gnu-efi/code gnu-efi \
    && cd gnu-efi \
    && git checkout ${GNU_EFI_VERSION} \
    && make

COPY --from=gcc /opt/cross /opt/cross
WORKDIR /novos
COPY . .

RUN make build TOOLPATH="${PREFIX}/bin/"

####################
# Export artifacts #
####################

FROM scratch AS export
COPY --from=builder /novos/bin /
