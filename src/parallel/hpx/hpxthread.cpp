// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Peter Doak (doakpw@ornl.gov)
//
// This file implements hpxthread.hpp.

#include <iostream>

#include "dca/parallel/hpx/hpxthread.hpp"
#include "hpx/runtime/get_worker_thread_num.hpp"

namespace dca {
namespace parallel {

constexpr char hpxthread::parallel_type_str_[];

std::ostream& operator<<(std::ostream& o, const hpxthread& c) {
  o << '\n'
    << "threading type:" << c.parallel_type_str_ << '\n'
    << "number of HPX worker threads:" << hpx::get_num_worker_threads();
  return o;
}

}  // namespace parallel
}  // namespace dca
