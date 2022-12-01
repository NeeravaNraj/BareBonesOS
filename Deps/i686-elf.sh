#!/bin/bash

# exit on error
set -e

# setup definitions
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# make dirs and enter toolchain dir
mkdir -p $PREFIX
rm -fr $HOME/toolchain-build # attempt to remove toolchain directory. If it doesn't exist, it does nothing.
mkdir -p $HOME/toolchain-build
cd $HOME/toolchain-build

if (!($PREFIX/bin/$TARGET-ld -v | grep 2.39 > /dev/null)) then
    echo -----------------
    echo Building binutils
    echo -----------------

    # fetch binutils code
    curl -OL https://ftp.gnu.org/gnu/binutils/binutils-2.39.tar.gz
    tar -xf binutils-2.39.tar.gz

    # build binutils
    cd binutils-2.39
    mkdir build
    cd build
    ../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make -j6
    make install
    cd ../..
else
    echo Binutils is up to date
fi

#currentver="$($PREFIX/bin/$TARGET-gcc -dumpversion)"
#requiredver="12.2.0"
#if [ "$(printf '%s\n' "$requiredver" "$currentver" | sort -V | head -n1)" = "$requiredver" ]; then 
#    echo GCC is up to date.
#else
echo ------------
echo Building gcc
echo ------------

# fetch gcc code
curl -OL https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.gz
tar -xf gcc-12.2.0.tar.gz

# build gcc
cd gcc-12.2.0
mkdir build
cd build
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j6
make all-target-libgcc -j6
make install-gcc
make install-target-libgcc
cd ../..
#fi

# clean-up
cd ..
rm -fr $HOME/toolchain-build

if (!(nasm --v | grep 2.15.05 > /dev/null)) then
    echo -------------
    echo Building nasm
    echo -------------

    # Prepare for nasm
    cd $HOME
    rm -fr nasm-build # attempt to remove nasm build dir. If it doesn't exist, it does nothing
    mkdir nasm-build
    cd nasm-build

    # fetch nasm code
    curl -OL https://github.com/netwide-assembler/nasm/archive/refs/tags/nasm-2.15.05.tar.gz
    tar -xf nasm-2.15.05.tar.gz

    # build nasm
    cd nasm-nasm-2.15.05
    sh autogen.sh
    sh configure --prefix="$HOME/opt/nasm" --exec-prefix="$HOME/opt/nasm" --host=$TARGET
    make -j6
    make install
    cd ..

    # clean-up
    cd ..
    rm -fr $HOME/nasm-build
else
    echo NASM is up to date
fi
