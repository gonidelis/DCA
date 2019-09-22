#!/bin/bash
#
# Loads all modules that are required to build DCA++ on ORNL's Summit supercomputer.
# A reset is done at the beginning to restore to the default programming environment on Summit.
#
# Usage: source summit_load_modules.sh

module reset
module load gcc/8.1.1
module load cuda
module load hdf5
module load fftw
module load cmake
module load magma
export OLCF_MAGMA_ROOT="/gpfs/alpine/cph102/proj-shared/lib/magma-2.5.1-gcc8"
module load netlib-lapack
module load essl
#module load openblas/0.3.6-omp
module load spectrum-mpi/10.3.0.1-20190611

export CC=mpicc
export CXX=mpicxx
export CFLAGS=-fPIC
#export CXXFLAGS="-fPIC -mcpu=power9 -mtune=power9 -ffast-math -std=c++17"
export CXXFLAGS="-fPIC -mcpu=power9 -mtune=power9 -ffast-math"
export LDFLAGS=-dynamic
#export LDCXXFLAGS="$LDFLAGS -std=c++17"