#!/bin/bash
cd build
rm -r !(*.ttf)
cmake ..
make
./game