# HPX

if (NOT DCA_HAVE_HPX)
    message(FATAL_ERROR "HPX library not found but requested.")
endif()

configure_file("${PROJECT_SOURCE_DIR}/include/dca/config/hpx_defines.hpp.in"
        "${CMAKE_BINARY_DIR}/include/dca/config/hpx_defines.hpp"
        @ONLY)

#add_library(parallel_hpxthread STATIC hpxthread.cpp)
add_library(parallel_hpxthread STATIC hpxthread.cpp
        hpx_thread_pool/hpx_thread_pool.cpp hpx_thread_pool/affinity.cpp)