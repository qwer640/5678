FROM ubuntu:14.10

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y clang git unzip wget libc++-dev
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y binutils make automake libtool curl

RUN git config --global user.email "docker@example.com"
RUN git config --global user.name docker

ADD documentation/ /opt/principia/documentation/

WORKDIR /opt/principia/
RUN git clone "https://github.com/google/protobuf.git" --depth 1 -b "v3.0.0-alpha-1"
WORKDIR /opt/principia/protobuf
RUN git am "../documentation/Setup Files/protobuf.patch"
RUN ./autogen.sh
RUN ./configure CC=clang CXX=clang++ CXXFLAGS='-fPIC -std=c++11 -stdlib=libc++ -O3 -g' LDFLAGS='-stdlib=libc++'
RUN make -j 8 install

WORKDIR /opt/principia/
RUN git clone https://github.com/Norgg/glog
WORKDIR /opt/principia/glog
# RUN patch -p 1 -i "../documentation/Setup Files/glog.patch"; true
RUN ./configure CC=clang CXX=clang++ CXXFLAGS='-fPIC -std=c++11 -stdlib=libc++ -O3 -g' LDFLAGS='-stdlib=libc++'
RUN make -j 8 install

WORKDIR /opt/principia/
RUN wget https://googlemock.googlecode.com/files/gmock-1.7.0.zip
RUN unzip gmock-1.7.0.zip
WORKDIR /opt/principia/gmock-1.7.0
RUN patch -p 1 -i "../documentation/Setup Files/gmock.patch"; true
RUN ./configure CC=clang CXX=clang++ CXXFLAGS='-fPIC -std=c++11 -stdlib=libc++ -O3 -g' LDFLAGS='-stdlib=libc++'
RUN make -j 8

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y cmake
WORKDIR /opt/principia
RUN git clone https://github.com/pleroy/benchmark
WORKDIR /opt/principia/benchmark
RUN cmake .
RUN make

WORKDIR /opt/principia
RUN git clone "https://chromium.googlesource.com/chromium/src.git" chromium -n --depth 1 -b "40.0.2193.1"
# $GitPromptSettings.RepositoriesInWhichToDisableFileStatus += join-path  (gi -path .).FullName chromium
WORKDIR /opt/principia/chromium
RUN git config core.sparsecheckout true
RUN cp "../documentation/Setup Files/chromium_sparse_checkout.txt" .git/info/sparse-checkout
RUN git checkout
RUN git am "../documentation/Setup Files/chromium.patch"

WORKDIR /opt/principia/src
CMD make
