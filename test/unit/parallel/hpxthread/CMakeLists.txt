# hpxthread unit tests

dca_add_gtest(hpxthread_test GTEST_MAIN HPX LIBS parallel_hpxthread)
hpx_setup_target(hpxthread_test COMPONENT_DEPENDENCIES iostreams)
