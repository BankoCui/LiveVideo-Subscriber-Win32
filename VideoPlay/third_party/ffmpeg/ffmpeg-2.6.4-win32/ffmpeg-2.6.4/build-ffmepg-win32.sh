#! /bin/bash
echo "Build ffmpeg for h264 encoder with x264"
echo "Build ffmpeg for h264 decoder"

./configure \
--disable-shared \
--disable-swresample \
--disable-avresample \
--enable-swscale \
--disable-avformat \
--disable-avfilter \
--disable-pthreads \
--disable-ffprobe \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffserver \
--disable-network \
--disable-doc \
--disable-debug \
--disable-symver \
--disable-postproc \
--disable-stripping \
--disable-iconv \
--enable-small \
--enable-version3 \
--enable-gpl \
--enable-libx264 \
--enable-encoder=libx264 \
--enable-decoder=h264 \
--enable-parser=h264 \
--enable-decoder=hevc \
--enable-static \
--enable-memalign-hack \
--cpu=i686 \
--extra-cflags=-fno-common \
--prefix=$PWD/xiaohang/ffmpeg-2.6.4/
