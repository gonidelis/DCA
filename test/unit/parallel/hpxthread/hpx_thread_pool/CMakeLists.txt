# thread pool unit tests

find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
dca_add_gtest(hpxthread_pool_test GTEST_MAIN LIBS HPX parallel_hpxthread)

hpx_setup_target(hpxthread_pool_test COMPONENT_DEPENDENCIES iostreams)
dca_add_gtest(hpxaffinity_test GTEST_MAIN LIBS parallel_hpxthread)
