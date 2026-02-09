#!/usr/bin/sh
cur_dir=$PWD
mkdir osqp
git clone https://github.com/robotology/osqp-eigen.git
cd osqp-eigen
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=${cur_dir}/PrSBC/src/osqp ..
make
make install

cd .. # -> osqp-eigen
cd .. # -> repo
cd PrSBC/src
git clone https://gitlab.com/libeigen/eigen.git
# rm -rf osqp-eigen
