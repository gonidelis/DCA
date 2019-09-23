/*******************************************************************************
 *
 * Prints Type and as well as ref-ness / const-ness / volatile-ness
 * Programming Techniques for Scientific Simulations II, ETH Zürich, 2015
 * For free use, no rights reserved.
 *
 * Truncated and modified by Urs R. Haehner (haehneru@itp.phys.ethz.ch).
 *
 ******************************************************************************/

#ifndef DCA_UTIL_PRINT_TYPE_HPP
#define DCA_UTIL_PRINT_TYPE_HPP

#if !defined(_MSC_VER)
#include <cxxabi.h>
#endif
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>

namespace dca {
namespace util {
namespace detail {
// dca::util::detail::

inline std::string demangle(std::string const& str, int& status) {
#if !defined(_MSC_VER)
  std::unique_ptr<char, void (*)(void*)> dmgl(abi::__cxa_demangle(str.c_str(), 0, 0, &status),
                                              std::free);
  return (status == 0) ? dmgl.get() : str;
#else
  return str;
#endif
}

}  // detail

template <typename T>
struct Type {
  static std::string print() {
    std::stringstream ss;
    int status;
    ss << detail::demangle(typeid(T).name(), status);
    return ss.str();
  }
};

}  // util
}  // dca

#endif  // DCA_UTIL_PRINT_TYPE_HPP
