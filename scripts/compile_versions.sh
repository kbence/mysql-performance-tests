#!/bin/bash -ex

SOURCES="
    https://dev.mysql.com/get/Downloads/MySQL-5.7/mysql-5.7.21.tar.gz
    https://dev.mysql.com/get/Downloads/MySQL-5.6/mysql-5.6.39.tar.gz
"

for src in $SOURCES; do
    filename="$(basename "$src")"

    wget "$src"
    tar xf "$filename"

    pushd "${filename%%.tar.gz}"
        mkdir build
        pushd build
            cmake -DWITHOUT_SERVER=1 -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -g 'Unix Makefiles' ..
            make -j4
        popd
    popd

    rm -f "$filename"
done
