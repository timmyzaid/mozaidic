FROM debian

RUN apt-get update && apt-get install -y \
        libcurl3 \
        libgtk2.0-0 \
        libjasper-dev \
        libdc1394-22 \
        libva1 \
        libzvbi0 \
        libxvidcore4 \
        libwebp5 \
        libwavpack1 \
        libvorbisenc2 \
        libvo-amrwbenc0 \
        libvo-aacenc0 \
        libtwolame0 \
        libtheora0 \
        libspeex1 \
        libsnappy1 \
        libshine3 \
        libschroedinger-1.0-0 \
        libopus0 \
        libopenjpeg-dev \
        libopencore-amrwb0 \
        libopencore-amrnb0 \
        libmp3lame-dev \
        libgsm1 \
        libcrystalhd3 \
        libssh-gcrypt-4 \
        libmodplug1 \
        libgme0 \
        libbluray1 \
        libsoxr0 \
        libnuma-dev \
     && rm -rf /var/lib/apt/lists/*

COPY build/libboost_system.so.1.61.0 /usr/local/lib/
COPY build/librestclient-cpp.so.1 /usr/local/lib/
COPY build/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/
COPY build/libopencv_highgui.so.2.4 /usr/local/lib/
COPY build/libopencv_core.so.2.4 /usr/local/lib/
COPY build/libopencv_imgproc.so.2.4 /usr/local/lib/
COPY build/libjpeg.so.8 /usr/lib/x86_64-linux-gnu/
COPY build/libavcodec-ffmpeg.so.56 /usr/lib/x86_64-linux-gnu/
COPY build/libavformat-ffmpeg.so.56 /usr/lib/x86_64-linux-gnu/
COPY build/libavutil-ffmpeg.so.54 /usr/lib/x86_64-linux-gnu/
COPY build/libswscale-ffmpeg.so.3 /usr/lib/x86_64-linux-gnu/
COPY build/libswresample-ffmpeg.so.1 /usr/lib/x86_64-linux-gnu/
COPY build/libx265.so.79 /usr/lib/x86_64-linux-gnu/
COPY build/libx264.so.148 /usr/lib/x86_64-linux-gnu/
COPY build/libvpx.so.3 /usr/lib/x86_64-linux-gnu/
COPY build/libgnutls.so.30 /usr/lib/x86_64-linux-gnu/
COPY build/libnettle.so.6 /usr/lib/x86_64-linux-gnu/
COPY build/libhogweed.so.4 /usr/lib/x86_64-linux-gnu/
COPY build/run /

ENTRYPOINT ./run