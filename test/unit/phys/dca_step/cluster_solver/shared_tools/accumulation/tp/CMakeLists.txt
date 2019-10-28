# test/unit/phys/dca_step/cluster_solver/shared_tools/tp
find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
include_directories(${HPX_INCLUDE_DIRS})
link_directories(${HPX_LIBRARY_DIR})

dca_add_gtest(tp_accumulator_test
  FAST
  GTEST_MAIN
  HPX
  INCLUDE_DIRS ${DCA_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}
  LIBS     ${DCA_LIBS}
  )

dca_add_gtest(tp_accumulator_singleband_test
  FAST
  GTEST_MAIN
  HPX
  INCLUDE_DIRS ${DCA_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}
  LIBS     ${DCA_LIBS}
  )

dca_add_gtest(tp_accumulator_particle_hole_test
  FAST
  GTEST_MAIN
    HPX
  INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${FFTW_INCLUDE_DIR} ${HDF5_INCLUDE_DIR} ${HPX_INCLUDE_DIRS}
  LIBS function cluster_domains time_and_frequency_domains quantum_domains timer json random
       enumerations dca_algorithms parallel_hpxthread parallel_util ${LAPACK_LIBRARIES})

dca_add_gtest(tp_accumulator_gpu_test
  CUDA
  GTEST_MAIN
  HPX
  INCLUDE_DIRS ${DCA_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}
  LIBS     ${DCA_LIBS}
  )

dca_add_gtest(tp_accumulator_singleband_gpu_test
  CUDA
  GTEST_MAIN
  HPX
  INCLUDE_DIRS ${DCA_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}
  LIBS     ${DCA_LIBS}
  )

add_subdirectory(ndft)
