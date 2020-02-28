#!/bin/bash
set -x
mode=${1:-"all"} # all: configure+make+install; or make+install
prefix=output
if [ ! -d output ]; then
    mkdir -p output
else
    rm -rf output/*
fi

make clean

#./configure --prefix=./output --enable-shared --enable-pthreads --enable-version3 --enable-hardcoded-tables --enable-avresample --cc=clang --enable-ffplay --enable-gnutls --enable-gpl --enable-libbluray --enable-libmp3lame --enable-libopus --enable-librubberband --enable-libsnappy --enable-libtesseract --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libxvid --enable-lzma --enable-libfontconfig --enable-libfreetype --enable-frei0r --enable-libass --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libspeex --enable-videotoolbox  --enable-libsoxr --enable-debug --disable-x86asm
if [ "${mode}" == "all" ]; then
    ./configure --enable-shared --enable-pthreads --enable-version3 --enable-hardcoded-tables --enable-avresample --cc=clang --enable-ffplay --enable-gnutls --enable-gpl --enable-libbluray --enable-libmp3lame --enable-libopus --enable-librubberband --enable-libsnappy --enable-libtesseract --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libxvid --enable-lzma --enable-libfontconfig --enable-libfreetype --enable-frei0r --enable-libass --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libspeex --enable-videotoolbox  --enable-libsoxr --enable-debug --disable-x86asm
fi

sudo make
sudo make install
