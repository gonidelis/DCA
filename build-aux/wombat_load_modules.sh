#!/bin/bash
#
# Loads all modules that are required to build DCA++ on ORNL's Summit supercomputer.
# A reset is done at the beginning to restore to the default programming environment on Summit.
#
# Usage: source summit_load_modules.sh

module purge 
module load cuda/10.2
module load fftw/fftw-3.3.8-gcc-8.2.1
module load openmpi/4.0.2rc3-gcc-8.2.1 
module load hdf5/1.10.5

export CC=mpicc
export CXX=mpicxx
