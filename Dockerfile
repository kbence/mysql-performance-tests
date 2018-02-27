FROM ubuntu:artful

WORKDIR /usr/src

RUN apt-get update && \
    apt-get install -y build-essential wget cmake libbz2-dev git netcat libncurses5-dev && \
    rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

RUN git clone https://github.com/boostorg/boost /usr/src/boost --branch boost-1.59.0 && \
    cd /usr/src/boost && git submodule update --init --jobs 8
# For some reason (it's probably a missing .gitattribute or something) the file
# /usr/src/boost/tools/build/src/util/option.jam gets corrupted during submodule update.
# Cleaning and checking out everything again fixes the issue.
RUN cd /usr/src/boost/tools/build && git clean -df && git checkout HEAD -- .
RUN cd boost && ./bootstrap.sh && ./b2 headers && ./b2 install -q

COPY ./scripts/compile_versions.sh /opt/

RUN /opt/compile_versions.sh

COPY ./scripts/run_tests.sh /opt/
COPY ./tests /opt/tests
CMD ["bash", "/opt/run_tests.sh"]
