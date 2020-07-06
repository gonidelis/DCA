# Initial cache list for Piz Daint GPU (Cray XC50)
#
# Usage: cmake /path/to/dca/source -C /path/to/this/file -D<option>=<value> ...

# Prevent CMake from searching for BLAS and LAPACK libraries.
# CC automatically links against them.
set(DCA_HAVE_LAPACK TRUE CACHE INTERNAL "")

# Use srun for executing the tests.
set(TEST_RUNNER "srun" CACHE STRING "Command for executing (MPI) programs.")

# Enable the GPU support.
option(DCA_WITH_CUDA "Enable GPU support." ON)

set(LAPACK_LIBRARIES /users/wweile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.1.0/openblas-0.3.10-pk5dankrufpycvdlpzp5uq2hyfckszde/lib/libopenblas.so;/users/wweile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.1.0/netlib-lapack-3.8.0-lgp23xttcbhrgouldw46edqqdxu6fcdl/lib64/libblas.so CACHE FILEPATH "Libraries to link against to use LAPACK.")

# Compile for Tesla compute architecture.
# set(CUDA_GPU_ARCH "sm_60" CACHE STRING "Name of the *real* architecture to build for.")  # default

# For the GPU support we also need MAGMA.
# MAGMA has been installed with EasyBuild.

# For the GPU support we also need MAGMA.
set(MAGMA_DIR  /users/wweile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.1.0/magma-2.5.1-jihs7dsvwlxvrsnivngcexvfci74s5gp CACHE PATH
  "Path to the MAGMA installation directory. Hint for CMake to find MAGMA.")

# FFTW paths.
set(FFTW_INCLUDE_DIR /users/wweile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.1.0/fftw-3.3.8-zdmx3pf5pjmne7hxx5oyfa5ze6bkimj6/include CACHE PATH "Path to fftw3.h.")
set(FFTW_LIBRARY /users/wweile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.1.0/fftw-3.3.8-zdmx3pf5pjmne7hxx5oyfa5ze6bkimj6/lib/libfftw3.so CACHE FILEPATH "The FFTW3(-compatible) library.")

# HDF5 paths
set(HDF5_INCLUDE_DIRS /opt/cray/pe/hdf5/1.10.5.1/GNU/7.1/include CACHE PATH "Path to hdf5 includes.")
#set(HDF5_INCLUDE_DIR /users/wweile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.1.0/hdf5-1.10.6-szhckfhpdpedvbjmb3djkbb2ypjddr5w/include CACHE PATH "Path to hdf5 includes.")
