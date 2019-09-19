# symmetrization

dca_add_gtest(symmetrize_test
        FAST
        GTEST_MAIN
        INCLUDE_DIRS ${FFTW_INCLUDE_DIR}
        HPX
        LIBS  ${DCA_LIBS}
        )
