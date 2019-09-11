# thread pool unit tests
find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
include_directories(${HPX_INCLUDE_DIRS})
link_directories(${HPX_LIBRARY_DIR})

add_executable(hpxthread_pool_test hpxthread_pool_test.cpp)
target_compile_definitions(hpxthread_pool_test PUBLIC -DHPX_DEBUG)
hpx_setup_target(hpxthread_pool_test
        DEPENDENCIES parallel_hpxthread gtest_main ${DCA_ADD_GTEST_LIBS}
        COMPONENT_DEPENDENCIES iostreams
        )

#find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
#dca_add_gtest(hpxthread_pool_test
#        GTEST_MAIN
#        HPX
#        LIBS parallel_hpxthread)

#hpx_setup_target(hpxthread_pool_test COMPONENT_DEPENDENCIES iostreams)
#dca_add_gtest(hpxaffinity_test GTEST_MAIN LIBS parallel_hpxthread)
