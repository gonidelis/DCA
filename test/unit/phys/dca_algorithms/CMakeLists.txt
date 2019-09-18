# DCA algorithms unit tests
find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
include_directories(${HPX_INCLUDE_DIRS})
link_directories(${HPX_LIBRARY_DIR})

dca_add_gtest(compute_band_structure_test
  GTEST_MAIN
  INCLUDE_DIRS ${FFTW_INCLUDE_DIR}
  LIBS dca_algorithms json function cluster_domains enumerations time_and_frequency_domains quantum_domains
  ${LAPACK_LIBRARIES} ${DCA_CUDA_LIBS})

dca_add_gtest(compute_free_greens_function_test
  GTEST_MAIN
  HPX
  INCLUDE_DIRS ${FFTW_INCLUDE_DIR} ${HPX_INCLUDE_DIRS}
  LIBS function parallel_hpxthread parallel_util ${LAPACK_LIBRARIES} ${DCA_CUDA_LIBS})

dca_add_gtest(compute_greens_function_test
  GTEST_MAIN
  HPX
  INCLUDE_DIRS ${FFTW_INCLUDE_DIR} ${HPX_INCLUDE_DIRS}
  LIBS function parallel_hpxthread parallel_util ${LAPACK_LIBRARIES} ${DCA_CUDA_LIBS})
