#!/bin/bash
#
# Loads all modules that are required to build DCA++ on ORNL's Summit supercomputer.
# A reset is done at the beginning to restore to the default programming environment on Summit.
#
# Usage: source summit_load_modules.sh

GCC_version=9.1.0
CMAKE_version=3.15.2
BOOST_VER=1.68.0

module reset
module unload gcc
module unload boost
module load gcc/$GCC_version
module load cmake/$CMAKE_version
module load cuda

export CC=gcc
export CXX=g++
export CFLAGS=-fPIC
export CXXFLAGS="-fPIC -mcpu=power9 -mtune=power9 -ffast-math -std=c++17"
export LDFLAGS=-dynamic
export LDCXXFLAGS="$LDFLAGS -std=c++17"

module load cuda
module load hdf5
module load fftw
export OLCF_MAGMA_ROOT=/gpfs/alpine/cph102/proj-shared/lib/magma-2.5.1
module load netlib-lapack
module load essl
