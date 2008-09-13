/* Implementation of the C interface: declarations.
   Copyright (C) 2001-2008 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#ifndef PPL_ppl_c_implementation_defs_hh
#define PPL_ppl_c_implementation_defs_hh 1

#define PPL_NO_AUTOMATIC_INITIALIZATION
#include "ppl.hh"
#include "ppl_c.h"
#include <stdexcept>
#include <limits>
#include <sstream>
#include <cstdio>
#include <cerrno>
#include <climits>

namespace Parma_Polyhedra_Library {

namespace C_Interface {

extern "C" typedef void
(*error_handler_type)(enum ppl_enum_error_code code, const char* description);

extern error_handler_type user_error_handler;

void notify_error(enum ppl_enum_error_code code, const char* description);

Relation_Symbol relation_symbol(enum ppl_enum_Constraint_Type t);

class PIFunc {
private:
  //! Holds the vector implementing the map.
  dimension_type* vec;

  //! Holds the size of \p vec.
  size_t vec_size;

  //! Cache for computing the maximum dimension in the codomain.
  mutable dimension_type max_in_codomain_;

  //! Cache for computing emptiness:
  //! -1 if we still don't know, 0 if not empty, 1 if empty.
  mutable int empty;

public:
  PIFunc(dimension_type* v, size_t n)
    : vec(v), vec_size(n), max_in_codomain_(not_a_dimension()), empty(-1) {
  }

  bool has_empty_codomain() const {
    if (empty < 0) {
      empty = 1;
      for (size_t i = vec_size; i-- > 0; )
	if (vec[i] != not_a_dimension()) {
	  empty = 0;
	  break;
	}
    }
    return empty;
  }

  dimension_type max_in_codomain() const {
    if (max_in_codomain_ == not_a_dimension()) {
      for (size_t i = vec_size; i-- > 0; ) {
	dimension_type vec_i = vec[i];
	if (vec_i != not_a_dimension()
	    && (max_in_codomain_ == not_a_dimension()
		|| vec_i > max_in_codomain_))
	  max_in_codomain_ = vec_i;
      }
    }
    return max_in_codomain_;
  }

  bool maps(dimension_type i, dimension_type& j) const {
    if (i >= vec_size)
      return false;
    dimension_type vec_i = vec[i];
    if (vec_i == not_a_dimension())
      return false;
    j = vec_i;
    return true;
  }
};

} // namespace C_Interface

} // namespace Parma_Polyhedra_Library


#define CATCH_STD_EXCEPTION(exception, code) \
catch (const std::exception& e) {	     \
  notify_error(code, e.what()); \
  return code; \
}

#define CATCH_ALL \
CATCH_STD_EXCEPTION(bad_alloc, PPL_ERROR_OUT_OF_MEMORY) \
CATCH_STD_EXCEPTION(invalid_argument, PPL_ERROR_INVALID_ARGUMENT) \
CATCH_STD_EXCEPTION(domain_error, PPL_ERROR_DOMAIN_ERROR) \
CATCH_STD_EXCEPTION(length_error, PPL_ERROR_LENGTH_ERROR) \
CATCH_STD_EXCEPTION(overflow_error, PPL_ARITHMETIC_OVERFLOW) \
CATCH_STD_EXCEPTION(runtime_error, PPL_ERROR_INTERNAL_ERROR) \
CATCH_STD_EXCEPTION(exception, PPL_ERROR_UNKNOWN_STANDARD_EXCEPTION) \
catch (...) {						     \
  notify_error(PPL_ERROR_UNEXPECTED_ERROR, \
	       "completely unexpected error: a bug in the PPL"); \
  return PPL_ERROR_UNEXPECTED_ERROR; \
}

#define DECLARE_CONVERSIONS(Type, CPP_Type)             \
  inline const CPP_Type*                                \
  to_const(ppl_const_##Type##_t x) {                    \
    return reinterpret_cast<const CPP_Type*>(x);        \
  }                                                     \
                                                        \
  inline CPP_Type*                                      \
  to_nonconst(ppl_##Type##_t x) {                       \
    return reinterpret_cast<CPP_Type*>(x);              \
  }                                                     \
                                                        \
  inline ppl_const_##Type##_t                           \
  to_const(const CPP_Type* x) {                         \
    return reinterpret_cast<ppl_const_##Type##_t>(x);   \
  }                                                     \
                                                        \
  inline ppl_##Type##_t                                 \
  to_nonconst(CPP_Type* x) {                            \
    return reinterpret_cast<ppl_##Type##_t>(x);         \
  }

#define DEFINE_PRINT_FUNCTIONS(Type)                                    \
  int                                                                   \
  ppl_io_print_##Type(ppl_const_##Type##_t x) try {                     \
    using namespace IO_Operators;                                       \
    std::ostringstream s;                                               \
    s << *to_const(x);                                                  \
    if (puts(s.str().c_str()) < 0)                                      \
      return PPL_STDIO_ERROR;                                           \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL                                                             \
                                                                        \
  int                                                                   \
  ppl_io_fprint_##Type(FILE* stream, ppl_const_##Type##_t x) try {      \
    using namespace IO_Operators;                                       \
    std::ostringstream s;                                               \
    s << *to_const(x);                                                  \
    if (fputs(s.str().c_str(), stream) < 0)                             \
      return PPL_STDIO_ERROR;                                           \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL

#define DEFINE_ASCII_DUMP_FUNCTIONS(Type)                               \
  int                                                                   \
  ppl_##Type##_ascii_dump(ppl_const_##Type##_t x, FILE* stream) try {   \
    std::ostringstream s;                                               \
    to_const(x)->ascii_dump(s);                                         \
    if (fputs(s.str().c_str(), stream) < 0)                             \
      return PPL_STDIO_ERROR;                                           \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL

#define DEFINE_OUTPUT_FUNCTIONS(Type)           \
  DEFINE_PRINT_FUNCTIONS(Type)                  \
  DEFINE_ASCII_DUMP_FUNCTIONS(Type)


#include "ppl_c_implementation.inlines.hh"

#endif // !defined(PPL_ppl_c_implementation_defs_hh)
