FROM ubuntu

RUN apt-get update && apt-get install -y \
        build-essential \
        libxml2-dev \
        pkg-config \
        libjpeg-turbo8-dev \
        libgtk2.0-dev \
        libexif-dev \
        cmake \
        curl \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ENTRYPOINT /bin/bash