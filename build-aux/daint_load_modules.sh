MPI_version=7.7.10
# GCC env
module unload PrgEnv-cray
module load   PrgEnv-gnu
module unload gcc 
module load   gcc/8.1.0
module unload cray-libsci
module unload cray-mpich
module load   cray-mpich/$MPI_version
module load   daint-gpu
module unload intel
module load cray-hdf5/1.10.5.1 
module load cudatoolkit/10.1.105_3.27-7.0.1.1_4.1__ga311ce7

# cray compiler wrappers
export  CC=/opt/cray/pe/craype/default/bin/cc
export CXX=/opt/cray/pe/craype/default/bin/CC

