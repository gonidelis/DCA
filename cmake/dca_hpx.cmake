# Copyright (c) 2017 Hartmut Kaiser
# Copyright (c) 2019 Weile Wei (weilewei09@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# setup HPX as a dependency
function(dca_setup_hpx)
    if (NOT HPX_DIR AND HPX_ROOT)
        set(HPX_DIR ${HPX_ROOT}/lib/cmake/HPX)
    endif ()

    if (NOT HPX_DIR AND EXISTS "$ENV{HPX_DIR}")
        set(HPX_DIR $ENV{HPX_DIR})
    endif ()
    if (EXISTS "${HPX_DIR}")
        find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
        set(DCA_HAVE_HPX TRUE CACHE INTERNAL "" FORCE)
        dca_add_haves_define(DCA_HAVE_HPX)

        if (NOT HPX_FOUND)
            message(WARNING "HPX could not be found, please set HPX_DIR to help locating it.")
        endif ()

        # make sure that configured build type for DCA matches the one used for HPX
        if (NOT (${HPX_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE}))
            list(FIND ${CMAKE_BUILD_TYPE} ${HPX_BUILD_TYPE} __pos)
            if (${__pos} EQUAL -1)
                message( WARNING
                        "The configured CMAKE_BUILD_TYPE (${CMAKE_BUILD_TYPE}) is "
                        "different from the build type used for the found HPX "
                        "(HPX_BUILD_TYPE: ${HPX_BUILD_TYPE})")
            endif ()
        endif ()

        include_directories(${HPX_INCLUDE_DIRS})
        list(APPEND DCA_INCLUDE_DIRS ${HPX_INCLUDE_DIRS})
        link_directories(${HPX_LIBRARY_DIR})

        if (HPX_GIT_COMMIT)
            string(SUBSTRING ${HPX_GIT_COMMIT} 0 10 __hpx_git_commit)
            message("HPX version: " ${HPX_VERSION_STRING} "(${__hpx_git_commit})")
        else ()
            message("HPX version: " ${HPX_VERSION_STRING})
        endif ()
    else ()
        message("HPX_DIR has not been specified, please set it to help locating HPX")
    endif ()
endfunction()