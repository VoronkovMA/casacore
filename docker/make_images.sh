#!/bin/bash -ve

HERE=`dirname "$0"`
cd $HERE/..

for i in 37 38 39 310 311; do
    docker build -f docker/py${i}_wheel.docker . -t quay.io/casacore/casacore:master_wheel${i}
done
