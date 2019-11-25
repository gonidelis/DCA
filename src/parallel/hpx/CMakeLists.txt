# HPX

configure_file("../../../include/dca/config/hpx_defines.hpp.in"
        "${CMAKE_BINARY_DIR}/include/dca/config/hpx_defines.hpp"
        @ONLY)

add_library(parallel_hpx STATIC hpxthread.cpp)
if(DCA_HAVE_HPX)
target_link_libraries(parallel_hpx PRIVATE hpx)
endif()
