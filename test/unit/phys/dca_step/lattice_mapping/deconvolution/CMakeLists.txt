# (Phys) deconvolution unit tests

dca_add_gtest(deconvolution_routines_test
  GTEST_MAIN
  HPX
  INCLUDE_DIRS ${SIMPLEX_GM_RULE_INCLUDE_DIR}
  LIBS json function ${DCA_THREADING_LIBS} parallel_util cluster_domains time_and_frequency_domains quantum_domains gaussian_quadrature tetrahedron_mesh coarsegraining enumerations ${LAPACK_LIBRARIES} lapack ${DCA_CUDA_LIBS})
