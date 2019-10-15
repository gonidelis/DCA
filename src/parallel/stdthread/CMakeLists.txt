# parallel stdthread
add_library(parallel_stdthread STATIC stdthread.cpp
                                      thread_pool/thread_pool.cpp thread_pool/affinity.cpp)

if(MSVC)
  # Find HWLOC
  include(dca_hwloc)
  target_include_directories(parallel_stdthread PUBLIC ${HWLOC_INCLUDE_DIR})
  target_link_libraries(parallel_stdthread PUBLIC ${HWLOC_LIBRARY})
endif()
