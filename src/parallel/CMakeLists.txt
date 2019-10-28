# parallel

if (DCA_HAVE_MPI)
  add_subdirectory(mpi_concurrency)
endif(DCA_HAVE_MPI)

add_subdirectory(no_concurrency)
add_subdirectory(no_threading)
#add_subdirectory(stdthread)
if (DCA_HAVE_HPX)
  add_subdirectory(hpx)
endif()
add_subdirectory(util)
