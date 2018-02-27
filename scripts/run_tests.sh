#!/bin/bash

export LD_LIBRARY_PATH=/usr/local/lib

# wait for mysql to be available
hostname="${MYSQL_HOST:-localhost}"
while ! nc $hostname 3306 >/dev/null; do
    sleep .1
done

for mysql_dir in $(find /usr/src/ -maxdepth 1 -name 'mysql-*' -type d); do
    pushd "$mysql_dir/build"
    echo Installing $(basename "$mysql_dir")
    make install > /dev/null

    for test_dir in /opt/tests/*; do
        pushd "$test_dir"
        make clean run
        popd
    done

    xargs rm -f < install_manifest.txt
    popd
done
