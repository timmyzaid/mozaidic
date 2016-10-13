FROM ubuntu

RUN apt-get update && apt-get install -y \
        libjpeg-turbo8-dev \
        libgtk2.0-dev \
        libexif-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY build/libraries/libvips-cpp.so.42.6.1 /usr/local/lib/libvips-cpp.so.42
COPY build/libraries/libvips.so.42.6.1 /usr/local/lib/libvips.so.42
COPY build/libraries/libboost_system.so.1.61.0 /usr/local/lib/

COPY build/image_service /
ENTRYPOINT ./image_service