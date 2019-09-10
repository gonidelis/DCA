# hpxthread unit tests

dca_add_gtest(hpxthread_test LIBS HPX parallel_hpxthread)
hpx_setup_target(hpxthread_test COMPONENT_DEPENDENCIES iostreams parallel_hpxthread)

add_subdirectory(hpx_thread_pool)
