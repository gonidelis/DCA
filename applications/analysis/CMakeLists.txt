# Builds main_analysis.

if (DCA_BUILD_ANALYSIS)
  add_executable(main_analysis main_analysis.cpp)
  target_include_directories(main_analysis PRIVATE ${DCA_INCLUDE_DIRS})
  target_link_libraries(main_analysis PUBLIC ${DCA_LIBS} parallel_hpxthread)
  hpx_setup_target(main_analysis)
endif()
