#!/bin/bash

module purge
module unload binutils/2.30
module load gcc/8.1.0
module load cuda
module load hdf5
module load cmake/3.14.2
module load mpi/openmpi-x86_64
module load openblas

export CC=mpicc
export CXX=mpicxx
export CFLAGS=-fPIC
export CXXFLAGS="-fPIC -ffast-math -std=c++17"
export LDFLAGS=-dynamic
export LDCXXFLAGS="$LDFLAGS -std=c++17"

export FFTW_ROOT="/home/wwei/dev/install/fftw-3.3.8"
export OPENBLAS_ROOT="/opt/mn/openblas/0.3.2"
export LIB64_PATH="/lib64"
export OMP_NUM_THREADS=1
