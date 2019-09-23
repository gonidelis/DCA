// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Giovanni Balduzzi (gbalduzz@itp.phys.ethz.ch)
//
// This file provides allocators with pinned or mapped memory usable with std::vector.

#ifndef DCA_LINALG_UTIL_ALLOCATORS_ALIGNED_ALLOCATOR_HPP
#define DCA_LINALG_UTIL_ALLOCATORS_ALIGNED_ALLOCATOR_HPP

#if defined(_MSC_VER)
#include <malloc.h>
#endif

namespace dca {
namespace linalg {
namespace util {
// dca::linalg::util::

template <typename T>
class AlignedAllocator {
protected:
  T* allocate(std::size_t n) {
    if (!n)
      return nullptr;

    T* ptr = nullptr;
#if !defined(_MSC_VER)
    int err = posix_memalign((void**)&ptr, 128, n * sizeof(T));
    if (err)
      throw(std::bad_alloc());
#else
    ptr = (T*)_aligned_malloc(n * sizeof(T), 128);
    if (ptr == nullptr)
      throw(std::bad_alloc());
#endif
    return ptr;
  }

  void deallocate(T*& ptr, std::size_t /*n*/ = 0) noexcept {
#if !defined(_MSC_VER)
    free(ptr);
#else
    _aligned_free(ptr);
#endif
    ptr = nullptr;
  }
};

}  // util
}  // linalg
}  // dca

#endif  // DCA_LINALG_UTIL_ALLOCATORS_ALIGNED_ALLOCATOR_HPP
