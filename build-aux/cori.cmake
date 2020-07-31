# Initial cache list for Summit
#
# Usage: cmake -C /path/to/this/file /path/to/DCA/source -D<option>=<value> ...

# Prevent CMake from searching for BLAS and LAPACK libraries.
# Paths to IBM's ESSL (preferred) and NETLIB-LAPACK will be set manually.
set(DCA_HAVE_LAPACK TRUE CACHE INTERNAL "If set to TRUE, prevents CMake from searching for LAPACK.")
# To give ESSL precedence it needs to be specified before NETLIB.
set(LAPACK_LIBRARIES /global/homes/w/weile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.2.0/netlib-lapack-3.8.0-kf7iqzw6yyfdhx6d5seaiuh47p52dnos/lib64/liblapack.so;/global/homes/w/weile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.2.0/netlib-lapack-3.8.0-kf7iqzw6yyfdhx6d5seaiuh47p52dnos/lib64/libblas.so;/global/homes/w/weile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.2.0/openblas-0.3.8-535yv5vsv4ax6hi4ts44wvn6cvaifz4r/lib/libopenblas.so CACHE FILEPATH "Libraries to link against to use LAPACK.")

# Use jsrun for executing the tests.
set(TEST_RUNNER "jsrun" CACHE STRING "Command for executing (MPI) programs.")
set(MPIEXEC_NUMPROC_FLAG "-n" CACHE STRING
  "Flag used by TEST_RUNNER to specify the number of processes.")
# Use 1 resource set with 1 GPU and 5 cores for executing the tests.
set(MPIEXEC_PREFLAGS "-a 1 -g 1 -c 5" CACHE STRING
  "Flags to pass to TEST_RUNNER directly before the executable to run.")
# The flag "--smpiargs=none" is needed to execute tests with no MPI functionalities.
set(SMPIARGS_FLAG_NOMPI "--smpiargs=none" CACHE STRING
  "Spectrum MPI argument list flag for serial tests.")
# Let's keep this option in case we need it again in the future.
set(SMPIARGS_FLAG_MPI "" CACHE STRING "Spectrum MPI argument list flag for MPI tests.")

# Enable the GPU support.
option(DCA_WITH_CUDA "Enable GPU support." ON)

# Compile for Volta compute architecture.
set(CUDA_GPU_ARCH "sm_70" CACHE STRING "Name of the *real* architecture to build for.")

# Summit's static CUDA runtime is bugged.
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

# For the GPU support we also need MAGMA.
set(MAGMA_DIR /global/homes/w/weile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.2.0/magma-2.5.1-fl2v4ninpmptkw3ie5fi3amtjqpp2i6u/ CACHE PATH
  "Path to the MAGMA installation directory. Hint for CMake to find MAGMA.")

# FFTW paths.
set(FFTW_INCLUDE_DIR /global/homes/w/weile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.2.0/fftw-3.3.8-pe4ojk77bfzddbmfnqvl7hyid2vh4wh2/include/ CACHE PATH "Path to fftw3.h.")
set(FFTW_LIBRARY /global/homes/w/weile/dev/src/spack/opt/spack/cray-cnl7-haswell/gcc-8.2.0/fftw-3.3.8-pe4ojk77bfzddbmfnqvl7hyid2vh4wh2/lib/libfftw3.so CACHE FILEPATH "The FFTW3(-compatible) library.")

# HDF5 paths
set(HDF5_ROOT /global/homes/w/weile/dev/install/hdf5-1.10.4 CACHE FILEPATH "Path to hdf5 include.")

