// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//
// JSON character mapper.

#ifndef DCA_IO_JSON_JSON_PARSER_JSON_CHARACTER_MAPPER_HPP
#define DCA_IO_JSON_JSON_PARSER_JSON_CHARACTER_MAPPER_HPP

#include <iostream>
#include "dca/io/json/json_parser/json_enumerations.hpp"

#if !defined(_MSC_VER)
using json_istream = std::wistream;
using json_ifstream = std::wifstream;
using json_char = wchar_t;
#else
using json_istream = std::istream;
using json_ifstream = std::ifstream;
using json_char = char;
#endif

namespace dca {
namespace io {
// dca::io::

class JSON_character_mapper {
public:
  static JSON_character_class_type map_char_to_class(wchar_t widec);

  static bool is_white_space(JSON_character_class_type& nextClass);

  static wchar_t get_escaped_character(json_istream& inputStream);

private:
  static JSON_character_class_type ascii_class[128];
};

}  // io
}  // dca

#endif  // DCA_IO_JSON_JSON_PARSER_JSON_CHARACTER_MAPPER_HPP
