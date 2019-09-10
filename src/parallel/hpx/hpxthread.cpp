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

#include "dca/parallel/hpx/hpxthread.hpp"

namespace dca {
    namespace parallel {

        constexpr char hpxthread::parallel_type_str_[];

//        hpx::ostream& operator<<(hpx::ostream& o, const hpxthread& c) {
//            o << '\n'
//              << "threading type:" << c.parallel_type_str_ << '\n'
//              << "number of std::threads:" << HPXThreadPool::get_instance().size();
//            return o;
//        }

    }  // parallel
}  // dca
