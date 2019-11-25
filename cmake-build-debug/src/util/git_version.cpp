// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This file implements the GitVersion.

#include "dca/util/git_version.hpp"
#include <iostream>
#include <string>

namespace dca {
namespace util {
// dca::util::

const std::string GitVersion::git_log = "commit b874cf7c8f66efa7dc3d592f6eeb96c52916ab89\nMerge: e0eed7eb ba9b17bf\nAuthor: Peter Doak <PDoakORNL@users.noreply.github.com>\nDate:   Tue Oct 22 15:03:11 2019 -0400\n\n    Merge pull request #147 from CompFUSE/ct_int-configuration\n    \n    Ct int configuration\n";
const std::string GitVersion::git_status = "";

void GitVersion::print() {
  std::cout << "\n"
            << "********************************************************************************\n"
            << "**********                        Git Version                         **********\n"
            << "********************************************************************************\n"
            << "\n"
            << "Last commit:\n"
            << GitVersion::git_log << "\n"
            << "Working tree:\n"
            << GitVersion::git_status << std::endl;
}

std::string GitVersion::string() {
  return std::string("Last commit:\n" + GitVersion::git_log + "\nWorking tree:\n" +
                     GitVersion::git_status);
}

}  // util
}  // dca
