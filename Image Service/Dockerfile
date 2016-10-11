FROM debian

RUN apt-get update && apt-get install -y \
        awscli \
        libglib2.0-0 \
        libpango1.0-0 \
        libexif12 \
        libcurl3 \
        gdb \
    && rm -rf /var/lib/apt/lists/*

COPY build/libaws-cpp-sdk-transfer.so /usr/local/lib/
COPY build/libaws-cpp-sdk-s3.so /usr/local/lib/
COPY build/libaws-cpp-sdk-core.so /usr/local/lib/
COPY build/libboost_system.so.1.61.0 /usr/local/lib/
COPY build/libvips-cpp.so.42 /usr/local/lib/
COPY build/libvips.so.42 /usr/local/lib/
COPY build/libjpeg.so.8 /usr/lib/x86_64-linux-gnu/
COPY build/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/

COPY build/run /

ENTRYPOINT ./run