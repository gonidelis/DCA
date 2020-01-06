dca_add_gtest(distributed_tp_accumulator_gpu_test
  CUDA
  MPI MPI_NUMPROC 3
  GTEST_MAIN
  INCLUDE_DIRS ${DCA_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}
  LIBS     ${DCA_LIBS} parallel_mpi_concurrency
  )

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

dca_add_gtest(distributed_gpudirect_tp_accumulator_gpu_test
        CUDA
        MPI MPI_NUMPROC 3
        GTEST_MAIN
        INCLUDE_DIRS ${DCA_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}
        LIBS     ${DCA_LIBS} parallel_mpi_concurrency
        )