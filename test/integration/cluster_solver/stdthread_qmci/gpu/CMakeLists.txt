
dca_add_gtest(stdthread_ctaux_gpu_tp_test
    EXTENSIVE
    CUDA
    GTEST_MAIN
    HPX
    INCLUDE_DIRS ${DCA_INCLUDE_DIRS}
    LIBS ${DCA_LIBS}
    )

