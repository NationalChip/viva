#!/bin/bash

g++ -rdynamic -Wl,-rpath=/home/sundy/work/tools/openfst/lib  latgen-faster.o ../lm/lm.a ../decoder/decoder.a ../lat/lat.a ../cpucompute/cpucompute.a ../util/util.a ../base/base.a   -L/home/sundy/work/tools/openfst/lib -lfst /usr/lib/liblapack.so /usr/lib/libcblas.so /usr/lib/libatlas.so /usr/lib/libf77blas.so -lm -lpthread -ldl -o liblatgen-faster.so -fPIC -shared
