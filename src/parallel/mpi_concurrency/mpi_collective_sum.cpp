// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Weile Wei (wwei9@lsu.edu)
//
// This file implements mpi_collective_sum.hpp.

#include "dca/parallel/mpi_concurrency/mpi_collective_sum.hpp"


namespace dca {
namespace parallel {
// dca::parallel::
void MPICollectiveSum::mpi_wait(MPI_Request* request, MPI_Status* status) const
{
    MPI_Wait(request, status);
}

}  // namespace parallel
}  // namespace dca
