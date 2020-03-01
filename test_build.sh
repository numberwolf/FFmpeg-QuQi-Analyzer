#!/bin/bash
##### TEST BUILD FOR MAC
set -x
mode=${1:-"all"} # all: configure+make+install; or make+install
platform=${2:-"linux"} # mac win
prefix=output
if [ ! -d output ]; then
    mkdir -p output
else
    rm -rf output/*
fi

make clean

#./configure --prefix=./output --enable-shared --enable-pthreads --enable-version3 --enable-hardcoded-tables --enable-avresample --cc=clang --enable-ffplay --enable-gnutls --enable-gpl --enable-libbluray --enable-libmp3lame --enable-libopus --enable-librubberband --enable-libsnappy --enable-libtesseract --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libxvid --enable-lzma --enable-libfontconfig --enable-libfreetype --enable-frei0r --enable-libass --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libspeex --enable-videotoolbox  --enable-libsoxr --enable-debug --disable-x86asm
if [ "${mode}" == "all" ]; then
    if [ "${platform}" == "linux" ]; then
        # export LD_LIBRARY_PATH=$FFMPEGPATH/output/lib
      ./configure --prefix=/usr/local/ffmpeg --enable-gpl --enable-nonfree  --enable-debug --disable-optimizations --enable-shared --enable-pthreads
    else
#      ./configure --enable-shared --enable-pthreads --enable-version3 --enable-hardcoded-tables --enable-avresample --cc=clang --enable-ffplay --enable-gnutls --enable-gpl --enable-libbluray --enable-libmp3lame --enable-libopus --enable-librubberband --enable-libsnappy --enable-libtesseract --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libxvid --enable-lzma --enable-libfontconfig --enable-libfreetype --enable-frei0r --enable-libass --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libspeex --enable-videotoolbox  --enable-libsoxr --enable-debug --disable-x86asm
      ./configure --prefix=/usr/local/ffmpeg --enable-gpl --enable-nonfree  --enable-debug --disable-optimizations --enable-shared --enable-pthreads
    fi
fi

sudo make
sudo make install
