# Builds main_dca.

if (DCA_BUILD_DCA)
  add_executable(main_dca main_dca.cpp)
  target_include_directories(main_dca PRIVATE ${DCA_INCLUDE_DIRS})
  target_link_libraries(main_dca PRIVATE ${DCA_LIBS})

  if(DCA_HAVE_HPX)
    find_package(HPX REQUIRED)
    include_directories(${HPX_INCLUDE_DIRS})
    link_directories(${HPX_LIBRARY_DIR})
    hpx_setup_target(main_dca)
  endif()

  if (DCA_WITH_CUDA)
    set(CUDA_LINK_LIBRARIES_KEYWORD PUBLIC)
    cuda_add_cublas_to_target(main_dca)
  endif()
endif()
