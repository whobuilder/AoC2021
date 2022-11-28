FROM debian:buster

RUN apt-get update \
    && apt-get install -y g++ build-essential make cmake gdb gdbserver \
    rsync zip openssh-server git libopenblas-dev libboost-all-dev

# install dependecies
ADD OnKiCMakeFiles /OnKiCMakeFiles
RUN git clone https://github.com/ericniebler/range-v3.git --branch 0.11.0 --single-branch \
    && mkdir /range-v3/build && cd /range-v3/build \
    && cmake -DRANGE_V3_TESTS=OFF -DRANGE_V3_EXAMPLES=OFF -DCMAKE_BUILD_TYPE=Release .. \
    && make install -j4

RUN git clone https://github.com/fmtlib/fmt.git --branch 8.1.1 --single-branch \
    && mkdir /fmt/build && cd /fmt/build \
    && cmake -DFMT_TEST=OFF -DCMAKE_BUILD_TYPE=Release .. \
    && make install -j4

RUN git clone https://bitbucket.org/blaze-lib/blaze.git --branch v3.8.1 --single-branch \
    && mkdir /blaze/build && cd /blaze/build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
    && make install -j4


