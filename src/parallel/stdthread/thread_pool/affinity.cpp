// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Giovanni Balduzzi(gbalduzz@itp.phys.ethz.ch)
//
// This file implements the methods in affinity.hpp.

#include "dca/parallel/stdthread/thread_pool/affinity.hpp"

#include <iostream>
#include <cstdlib>

// GNU extensions are required for linux-specific features for querying affinity
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#if !defined(_MSC_VER)
#include <sched.h>
#else
#include <hwloc.h>
#endif

#include <stdexcept>

#if defined(_MSC_VER)
struct topology {
  topology() : topo(nullptr) {
    int err = hwloc_topology_init(&topo);
    if (err != 0) {
      throw(std::runtime_error("Unable to get thread topology information."));
    }
    err = hwloc_topology_ignore_type(topo, HWLOC_OBJ_PACKAGE);
    if (err != 0) {
      throw(std::runtime_error("hwloc_topology_ignore_type(HWLOC_OBJ_PACKAGE) failed."));
    }
    err = hwloc_topology_ignore_type(topo, HWLOC_OBJ_CACHE);
    if (err != 0) {
      throw(std::runtime_error("hwloc_topology_ignore_type(HWLOC_OBJ_CACHE) failed."));
    }
    err = hwloc_topology_ignore_type(topo, HWLOC_OBJ_GROUP);
    if (err != 0) {
      throw(std::runtime_error("hwloc_topology_ignore_type(HWLOC_OBJ_GROUP) failed."));
    }
    err = hwloc_topology_load(topo);
    if (err != 0) {
      throw(std::runtime_error("Unable to fill thread topology information."));
    }
  }
  ~topology() {
    if(topo) {
      hwloc_topology_destroy(topo);
    }
  }

  int get_number_of_cores() const {
    int nobjs = hwloc_get_nbobjs_by_type(topo, HWLOC_OBJ_CORE);
    // If num_cores is smaller 0, we have an error
    if (0 > nobjs) {
      throw(std::runtime_error("hwloc_get_nbobjs_by_type(HWLOC_OBJ_CORE) failed."));
    }
    else if (0 == nobjs) {
      // some platforms report zero cores but might still report the number of PUs
      nobjs = hwloc_get_nbobjs_by_type(topo, HWLOC_OBJ_PU);
      if (0 > nobjs) {
        throw(std::runtime_error("hwloc_get_nbobjs_by_type(HWLOC_OBJ_PU) failed."));
      }
    }

    // the number of reported cores/pus should never be zero either to
    // avoid division by zero, we should always have at least one core
    if (0 == nobjs) {
      throw(std::runtime_error("hwloc_get_nbobjs_by_type reports zero cores/pus."));
    }
    return nobjs;
  }

  int get_index(hwloc_obj_t obj) const {
    // on Windows logical_index is always -1
    if (obj->logical_index == ~0x0u)
      return obj->os_index;
    return obj->logical_index;
  }

  int get_cpubind_mask() const {
    hwloc_cpuset_t cpuset = hwloc_bitmap_alloc();

    int mask = 0;

    if (hwloc_get_cpubind(topo, cpuset, HWLOC_CPUBIND_PROCESS)) {
      hwloc_bitmap_free(cpuset);
      throw(std::runtime_error("hwloc_get_cpubind failed."));
    }

    int depth = hwloc_get_type_or_below_depth(topo, HWLOC_OBJ_CORE);
    int cores = get_number_of_cores();
    for (unsigned int i = 0; i != cores; ++i)
    {
      hwloc_obj_t core_obj = hwloc_get_obj_by_depth(topo, depth, i);
      unsigned idx = unsigned(get_index(core_obj));
      if (hwloc_bitmap_isset(cpuset, idx) != 0) {
        mask |= 0x1 << idx;
      }
    }
    hwloc_bitmap_free(cpuset);
    return mask;
  }

  void set_cpubind_mask(int mask) const {
    hwloc_cpuset_t cpuset = hwloc_bitmap_alloc();

    int depth = hwloc_get_type_or_below_depth(topo, HWLOC_OBJ_CORE);

    int cores = get_number_of_cores();
    for (int i = 0; i != cores; ++i) {
      if (mask & (0x1 << i)) {
        hwloc_obj_t core_obj = hwloc_get_obj_by_depth(topo, depth, unsigned(i));
        hwloc_bitmap_set(cpuset, unsigned(get_index(core_obj)));
      }
    }

    if (hwloc_set_cpubind(topo, cpuset, HWLOC_CPUBIND_STRICT | HWLOC_CPUBIND_PROCESS)) {
      // Strict binding not supported or failed, try weak binding.
      if (hwloc_set_cpubind(topo, cpuset, HWLOC_CPUBIND_PROCESS)) {
        hwloc_bitmap_free(cpuset);
        throw(std::runtime_error("hwloc_set_cpubind failed."));
      }
    }
    hwloc_bitmap_free(cpuset);
  }

  hwloc_topology_t topo;
};
#endif

namespace dca {
namespace parallel {
// dca::parallel::

std::vector<int> get_affinity() {
#if !defined(_MSC_VER)
  cpu_set_t cpu_set_mask;

  auto status = sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set_mask);

  if (status == -1) {
    throw(std::runtime_error("Unable to get thread affinity."));
  }

  auto cpu_count = CPU_COUNT(&cpu_set_mask);

  std::vector<int> cores;
  cores.reserve(cpu_count);

  for (auto i = 0; i < CPU_SETSIZE && cores.size() < cpu_count; ++i) {
    if (CPU_ISSET(i, &cpu_set_mask)) {
      cores.push_back(i);
    }
  }

  if (cores.size() != cpu_count) {
    throw(std::logic_error("Core count mismatch."));
  }

  return cores;
#else
  topology topo;

  int num_cores = topo.get_number_of_cores();
  std::vector<int> cores;
  cores.reserve(num_cores);

  int cpu_mask = topo.get_cpubind_mask();
  for (int i = 0; i != num_cores; ++i) {
    if (cpu_mask & (0x1 << i)) {
      cores.push_back(i);
    }
  }

  return cores;
#endif
}

void set_affinity(const std::vector<int>& cores) {
#if !defined(_MSC_VER)
  cpu_set_t cpu_set_mask;
  CPU_ZERO(&cpu_set_mask);

  for (int core : cores) {
    CPU_SET(core, &cpu_set_mask);
  }

  sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set_mask);
#else
  topology topo;
  int cpu_mask = 0;
  for (int core : cores) {
    cpu_mask |= 0x1 << core;
  }
  topo.set_cpubind_mask(cpu_mask);
#endif
}

int get_core_count() {
#if !defined(_MSC_VER)
  cpu_set_t cpu_set_mask;
  sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set_mask);
  return CPU_COUNT(&cpu_set_mask);
#else
  topology topo;
  return topo.get_number_of_cores();
#endif
}

}  // namespace parallel
}  // namespace dca
