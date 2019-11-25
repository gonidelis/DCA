# stdthread unit tests
if(NOT DCA_WITH_HPX)
dca_add_gtest(stdthread_test GTEST_MAIN LIBS parallel_stdthread)

add_subdirectory(thread_pool)

endif()