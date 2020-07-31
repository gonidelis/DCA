#!/bin/bash
#
# Loads all modules that are required to build DCA++ on ORNL's Summit supercomputer.
# A reset is done at the beginning to restore to the default programming environment on Summit.
#
# Usage: source summit_load_modules.sh

module purge 
module load gcc/8.2.0         
module load openmpi/4.0.2
module load cuda/10.1.243

export CC=mpicc
export CXX=mpicxx
