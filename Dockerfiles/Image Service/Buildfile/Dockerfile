FROM ubuntu

RUN apt-get update && apt-get install -y \
        build-essential \
        libxml2-dev \
        pkg-config \
        libjpeg-turbo8-dev \
        libgtk2.0-dev \
        libexif-dev \
        cmake \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY dependencies/vips-8.4.1.tar.gz dependencies/boost_1_61_0.tar.gz /
RUN tar -zxvf vips-8.4.1.tar.gz \
    && cd vips-8.4.1 && ./configure \
    && make -j8 \
    && make install

RUN tar -zxvf boost_1_61_0.tar.gz \
    && cd boost_1_61_0 \
    && chmod +x bootstrap.sh \
    && ./bootstrap.sh \
    && chmod +x b2 \
    && ./b2 install -j8; exit 0

RUN rm -rf boost_1_61_0 vips-8.4.1 \
    && rm boost_1_61_0.tar.gz \
    && rm vips-8.4.1.tar.gz

COPY build.sh /
RUN chmod +x /build.sh
ENTRYPOINT /bin/bash