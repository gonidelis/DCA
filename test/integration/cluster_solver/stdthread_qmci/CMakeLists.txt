dca_add_gtest(stdthread_ctaux_tp_test
    EXTENSIVE
    GTEST_MAIN
    HPX
    INCLUDE_DIRS ${DCA_INCLUDE_DIRS}
    LIBS         ${DCA_LIBS}
    )

add_subdirectory(gpu)
