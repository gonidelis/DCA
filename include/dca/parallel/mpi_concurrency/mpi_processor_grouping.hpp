// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//         Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//         Giovanni Balduzzi (gbalduzz@itp.phys.ethz.ch)
//
// This class manages the processor grouping for MPI.

#ifndef DCA_PARALLEL_MPI_CONCURRENCY_MPI_PROCESSOR_GROUPING_HPP
#define DCA_PARALLEL_MPI_CONCURRENCY_MPI_PROCESSOR_GROUPING_HPP

#include <cassert>
#include <vector>

#include <mpi.h>

namespace dca {
namespace parallel {
// dca::parallel::

class MPIProcessorGrouping {
public:
  // Creates a processor grouping. Only the processor able to pass the required check will have a
  // valid id.
  MPIProcessorGrouping(bool (*check)() = defaultCheck);

  ~MPIProcessorGrouping();

  MPI_Comm get() const {
    return MPI_communication_;
  }
  int get_id() const {
    assert(id_ > -1);
    return id_;
  }
  int get_size() const {
    assert(size_ > -1);
    return size_;
  }
  int get_world_id() const {
    assert(world_id_ > -1);
    return world_id_;
  }
  int get_world_size() const {
    assert(world_size_ > -1);
    return world_size_;
  }

  int first() const {
    return 0;
  }
  int last() const {
    return size_ - 1;
  }

  bool isValid() const {
    return id_ >= 0;
  }

#ifdef DCA_HAVE_CUDA_AWARE_MPI
  MPI_Comm get_row_comm() const {
    return row_comm_;
  }
  int get_row_id() const {
    assert(row_id_ > -1);
    return row_id_;
  }
  int get_row_size() const {
    assert(row_size_ > -1);
    return row_size_;
  }
  int get_color() const {
    assert(color_row_ > -1);
    return color_row_;
  }

  MPI_Comm get_col_comm() const {
    return col_comm_;
  }
  int get_col_id() const {
    assert(col_id_ > -1);
    return col_id_;
  }

  int get_col_size() const {
    assert(col_size_ > -1);
    return col_size_;
  }

  int get_col_first() const {
    return 0;
  }
#endif

private:
  // Checks if the processor is able to run a simple CUDA kernel.
  static bool defaultCheck();

  void printRemovedProcesses() const;

private:
  int id_ = -1;
  int size_ = -1;
  int world_id_ = -1;
  int world_size_ = -1;
  MPI_Comm MPI_communication_ = MPI_COMM_NULL;

#ifdef DCA_HAVE_CUDA_AWARE_MPI
  int color_row_ = -1;
  int row_size_ = -1;
  int row_id_ = -1;
  MPI_Comm row_comm_ = MPI_COMM_NULL;

  int color_col_ = -1;
  int col_size_ = -1;
  int col_id_ = -1;
  MPI_Comm col_comm_ = MPI_COMM_NULL;
#endif
};

}  // parallel
}  // dca

#endif  // DCA_PARALLEL_MPI_CONCURRENCY_MPI_PROCESSOR_GROUPING_HPP
