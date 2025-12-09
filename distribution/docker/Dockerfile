FROM debian:trixie

RUN apt-get update
RUN apt-get install -y git qt5-qmake g++ qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev libqt5svg5-dev libssl-dev make

ENV QTDIR=/usr/lib/x86_64-linux-gnu/qt5/

RUN mkdir /build
# fetch resources with git
RUN cd /build && \
	git clone https://github.com/R-Rudolph/OmegaRPG && \
        cd OmegaRPG && \
	git submodule update --init --recursive
# build and install
RUN mkdir /build/build && \
	cd /build/build && \
        /usr/lib/x86_64-linux-gnu/qt5/bin/qmake /build/OmegaRPG/OmegaRPG-Server-CLI/ && \
        make && \
        cp OmegaRPG-Server-CLI /usr/bin/OmegaRPG-Server-CLI
# cleanup
RUN rm -r /build
