/* Common part of the Prolog interfaces: declarations.
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

#ifndef PPL_ppl_prolog_common_defs_hh
#define PPL_ppl_prolog_common_defs_hh 1

#define PPL_NO_AUTOMATIC_INITIALIZATION
#include "ppl.hh"
#ifdef PPL_WATCHDOG_LIBRARY_ENABLED
#include "pwl.hh"
#endif
#include "ppl_prolog_sysdep.hh"
#include "interfaced_boxes.hh"
#include <set>
#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>

#ifndef PROLOG_TRACK_ALLOCATION
#define PROLOG_TRACK_ALLOCATION 0
#endif

namespace Parma_Polyhedra_Library {

namespace Prolog_Interfaces {

#if PROLOG_TRACK_ALLOCATION


class Poly_Tracker {
public:
  //! Construct an allocation tracker with no registered objects.
  Poly_Tracker();

  /*! \brief
    Register an object whose deletion is under the Prolog programmer
    responsibility.
  */
  void insert(const void* pp);

  /*! \brief
    Register an object whose deletion is under the PPL library
    responsibility.
  */
  void weak_insert(const void* pp);

  //! Check whether the object was correctly registered.
  void check(const void* pp) const;

  /*! \brief
    Unregister an object whose deletion is under the Prolog programmer
    responsibility.
  */
  void remove(const void* pp);

  /*! \brief
    Destroy the allocation tracker: an error message will be output
    if there still are registered objects whose deletion was under
    the Prolog programmer responsibility.
  */
  ~Poly_Tracker();

private:
  //! The type for recording a set of pointers to PPL library objects.
  typedef std::set<const void*, std::less<const void*> > Set;

  /*! \brief
    A set of pointers to objects whose deallocation is under the
    rensponsibility of the Prolog programmer: they should be deallocated
    before the termination of the program.
  */
  Set s;

  /*! \brief
    A set of pointers to objects whose deallocation is under the
    rensponsibility of the PPL library: they should not be deallocated
    by the Prolog programmer.
  */
  Set weak_s;
};

inline
Poly_Tracker::Poly_Tracker() {
}

inline
Poly_Tracker::~Poly_Tracker() {
  Set::size_type n = s.size();
  if (n > 0)
    std::cerr
      << "Poly_Tracker: " << n << " polyhedra leaked!"
      << std::endl;
}

inline void
Poly_Tracker::insert(const void* pp) {
  std::pair<Set::iterator, bool> stat = s.insert(pp);
  if (!stat.second) {
    std::cerr
      << "Poly_Tracker: two polyhedra at the same address at the same time?!"
      << std::endl;
    abort();
  }
}

inline void
Poly_Tracker::weak_insert(const void* pp) {
  weak_s.insert(pp);
}

inline void
Poly_Tracker::check(const void* pp) const {
  if (s.find(pp) == s.end()
      && weak_s.find(pp) == weak_s.end()) {
    std::cerr
      << "Poly_Tracker: attempt to access a nonexistent polyhedron."
      << std::endl;
    abort();
  }
}

void
Poly_Tracker::remove(const void* pp) {
  if (s.erase(pp) != 1) {
    std::cerr
      << "Poly_Tracker: attempt to deallocate a nonexistent polyhedron."
      << std::endl;
    abort();
  }
}

namespace {

inline Poly_Tracker&
poly_tracker() {
  static Poly_Tracker pt;
  return pt;
}

} // namespace

#define PPL_REGISTER(x) Parma_Polyhedra_Library::poly_tracker().insert(x)
#define PPL_WEAK_REGISTER(x) Parma_Polyhedra_Library::poly_tracker().weak_insert(x)
#define PPL_UNREGISTER(x) Parma_Polyhedra_Library::poly_tracker().remove(x)
#define PPL_CHECK(x) Parma_Polyhedra_Library::poly_tracker().check(x)

#else

#define PPL_REGISTER(x)
#define PPL_WEAK_REGISTER(x)
#define PPL_UNREGISTER(x)
#define PPL_CHECK(x)

#endif

class internal_exception {
private:
  Prolog_term_ref t;
  const char* w;

public:
  internal_exception(Prolog_term_ref term, const char* where)
    : t(term),
      w(where) {
  }

  virtual ~internal_exception() {
  }

  virtual Prolog_term_ref term() const {
    return t;
  }

  virtual const char* where() const {
    return w;
  }
};

class Prolog_unsigned_out_of_range : public internal_exception {
private:
  unsigned long m;

public:
  Prolog_unsigned_out_of_range(Prolog_term_ref term,
			       const char* where,
			       unsigned long max)
    : internal_exception(term, where),
      m(max) {
  }

  unsigned long max() const {
    return m;
  }
};

class non_linear : public internal_exception {
public:
  non_linear(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_an_integer : public internal_exception {
public:
  not_an_integer(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_unsigned_integer : public internal_exception {
public:
  not_unsigned_integer(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_a_variable : public internal_exception {
public:
  not_a_variable(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_an_optimization_mode : public internal_exception {
public:
  not_an_optimization_mode(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_a_complexity_class : public internal_exception {
public:
  not_a_complexity_class(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_universe_or_empty : public internal_exception {
public:
  not_universe_or_empty(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_a_relation : public internal_exception {
public:
  not_a_relation(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class not_a_nil_terminated_list : public internal_exception {
public:
  not_a_nil_terminated_list(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class PPL_integer_out_of_range {
private:
  Parma_Polyhedra_Library::Coefficient n;

public:
  PPL_integer_out_of_range(const Parma_Polyhedra_Library::Coefficient& value)
    : n(value) {
  }

  const Parma_Polyhedra_Library::Coefficient value() const {
    return n;
  }
};

class ppl_handle_mismatch : public internal_exception {
public:
  ppl_handle_mismatch(Prolog_term_ref term, const char* where)
    : internal_exception(term, where) {
  }
};

class unknown_interface_error {
private:
  const char* w;

public:
  unknown_interface_error(const char* s)
    : w(s) {
  }

  const char* where() const {
    return w;
  }
};

#if 0
int
Prolog_get_Coefficient(Prolog_term_ref t, Coefficient& n);

int
Prolog_unify_Coefficient(Prolog_term_ref t, const Coefficient& n);

int
Prolog_put_Coefficient(Prolog_term_ref t, const Coefficient& n);
#endif

#if 0
Prolog_atom out_of_memory_exception_atom;
#endif

// For Prolog lists.
extern Prolog_atom a_nil;

// For variables.
extern Prolog_atom a_dollar_VAR;

// For linear expressions.
extern Prolog_atom a_plus;
extern Prolog_atom a_minus;
extern Prolog_atom a_asterisk;

// To represent rational numbers as fractions.
extern Prolog_atom a_slash;

// For constraints.
extern Prolog_atom a_less_than;
extern Prolog_atom a_equal_less_than;
extern Prolog_atom a_equal;
extern Prolog_atom a_greater_than_equal;
extern Prolog_atom a_greater_than;

// For congruences.
extern Prolog_atom a_is_congruent_to;
extern Prolog_atom a_modulo;

// For generators.
extern Prolog_atom a_line;
extern Prolog_atom a_ray;
extern Prolog_atom a_point;
extern Prolog_atom a_closure_point;

// For grid_generators.
extern Prolog_atom a_grid_line;
extern Prolog_atom a_parameter;
extern Prolog_atom a_grid_point;

// For the relation between a polyhedron and a constraint.
extern Prolog_atom a_is_disjoint;
extern Prolog_atom a_strictly_intersects;
extern Prolog_atom a_is_included;
extern Prolog_atom a_saturates;

// For the relation between a polyhedron and a generator.
extern Prolog_atom a_subsumes;

// Denotes a closed interval boundary.
extern Prolog_atom a_c;

// Denotes the empty set such as the empty interval or polyhedron.
extern Prolog_atom a_empty;

#if 0
// Denotes the universe polyhedron.
Prolog_atom a_universe;

// Denotes the maximization mode for optimization problems.
Prolog_atom a_max;

// Denotes the minimization mode for optimization problems.
Prolog_atom a_min;

// Denote possible outcomes of MIP problems solution attempts.
Prolog_atom a_unfeasible;
Prolog_atom a_unbounded;
Prolog_atom a_optimized;
#endif

// Denotes an open interval boundary.
extern Prolog_atom a_o;

// Denotes the constructor that turns two boundaries into a proper interval.
extern Prolog_atom a_i;

// Denote the -infinity and +infinity interval boundaries.
extern Prolog_atom a_minf;
extern Prolog_atom a_pinf;

// Denote complexity classes.
extern Prolog_atom a_polynomial;
extern Prolog_atom a_simplex;
extern Prolog_atom a_any;

#if 0
// Default timeout exception atom.
Prolog_atom a_time_out;

// "Out of memory" exception atom.
Prolog_atom a_out_of_memory;
#endif

// Boolean constants.
extern Prolog_atom a_true;
extern Prolog_atom a_false;

#if 0
// To build exception terms.
Prolog_atom a_ppl_invalid_argument;
Prolog_atom a_ppl_overflow_error;
Prolog_atom a_ppl_domain_error;
Prolog_atom a_ppl_length_error;
Prolog_atom a_ppl_representation_error;
Prolog_atom a_expected;
Prolog_atom a_found;
Prolog_atom a_where;

struct {
  Prolog_atom* p_atom;
  const char* name;
} const prolog_interface_atoms[] = {
  { &a_nil,                      "[]" },

  { &a_dollar_VAR,               "$VAR" },

  { &a_plus,                     "+" },
  { &a_minus,                    "-" },
  { &a_asterisk,                 "*" },

  { &a_slash,                    "/" },

  { &a_equal,                    "=" },
  { &a_greater_than_equal,       ">=" },
  { &a_equal_less_than,          "=<" },
  { &a_greater_than,             ">" },
  { &a_less_than,                "<" },

  { &a_is_congruent_to,          "=:=" },
  { &a_modulo,                   "/" },

  { &a_line,                     "line" },
  { &a_ray,                      "ray" },
  { &a_point,                    "point" },
  { &a_closure_point,            "closure_point" },

  { &a_grid_line,                "grid_line" },
  { &a_parameter,                "parameter" },
  { &a_grid_point,               "grid_point" },

  { &a_is_disjoint,              "is_disjoint" },
  { &a_strictly_intersects,      "strictly_intersects" },
  { &a_is_included,              "is_included" },
  { &a_saturates,                "saturates" },

  { &a_subsumes,                 "subsumes" },

  { &a_c,                        "c" },

  { &a_empty,                    "empty" },
  { &a_universe,                 "universe" },

  { &a_max,                      "max" },
  { &a_min,                      "min" },

  { &a_unfeasible,               "unfeasible" },
  { &a_unbounded,                "unbounded" },
  { &a_optimized,                "optimized" },

  { &a_o,                        "o" },
  { &a_i,                        "i" },

  { &a_minf,                     "minf" },
  { &a_pinf,                     "pinf" },

  { &a_polynomial,               "polynomial" },
  { &a_simplex,                  "simplex" },
  { &a_any,                      "any" },

  { &a_time_out,                 "time_out" },
  { &a_out_of_memory,            "out_of_memory" },

  { &a_true,                     "true" },
  { &a_false,                    "false" },

  { &a_ppl_invalid_argument,     "ppl_invalid_argument" },
  { &a_ppl_overflow_error,       "ppl_overflow_error" },
  { &a_ppl_domain_error,         "ppl_domain_error" },
  { &a_ppl_length_error,         "ppl_length_error" },
  { &a_ppl_representation_error, "ppl_representation_error" },
  { &a_expected,                 "expected" },
  { &a_found,                    "found" },
  { &a_where,                    "where" }
};

Prolog_term_ref
Prolog_atom_term_from_string(const char* s) {
  Prolog_term_ref t = Prolog_new_term_ref();
  Prolog_put_atom(t, Prolog_atom_from_string(s));
  return t;
}
#endif

void
handle_exception(const Prolog_unsigned_out_of_range& e);

void
handle_exception(const not_unsigned_integer& e);

void
handle_exception(const non_linear& e);

void
handle_exception(const not_a_variable& e);

void
handle_exception(const not_an_integer& e);

void
handle_exception(const ppl_handle_mismatch& e);

void
handle_exception(const not_an_optimization_mode& e);

void
handle_exception(const not_a_complexity_class& e);

void
handle_exception(const not_universe_or_empty& e);

void
handle_exception(const not_a_relation& e);

void
handle_exception(const not_a_nil_terminated_list& e);

void
handle_exception(const PPL_integer_out_of_range& e);

void
handle_exception(const unknown_interface_error& e);

void
handle_exception(const std::overflow_error& e);

void
handle_exception(const std::length_error& e);

void
handle_exception(const std::bad_alloc&);

void
handle_exception(const std::exception& e);

void
handle_exception();

class timeout_exception : public Parma_Polyhedra_Library::Throwable {
public:
  void throw_me() const {
    throw *this;
  }
  int priority() const {
    return 0;
  }
  timeout_exception() {
  }
};

void
handle_exception(const timeout_exception&);

#define CATCH_ALL \
  catch (const Prolog_unsigned_out_of_range& e) { \
    handle_exception(e); \
  } \
  catch (const not_unsigned_integer& e) { \
    handle_exception(e); \
  } \
  catch (const non_linear& e) { \
    handle_exception(e); \
  } \
  catch (const not_a_variable& e) { \
    handle_exception(e); \
  } \
  catch (const not_an_integer& e) { \
    handle_exception(e); \
  } \
  catch (const ppl_handle_mismatch& e) { \
    handle_exception(e); \
  } \
  catch (const not_an_optimization_mode& e) {	\
    handle_exception(e); \
  } \
  catch (const not_a_complexity_class& e) { \
    handle_exception(e); \
  } \
  catch (const not_universe_or_empty& e) { \
    handle_exception(e); \
  } \
  catch (const not_a_relation& e) { \
    handle_exception(e); \
  } \
  catch (const not_a_nil_terminated_list& e) { \
    handle_exception(e); \
  } \
  catch (const PPL_integer_out_of_range& e) { \
    handle_exception(e); \
  } \
  catch (const unknown_interface_error& e) { \
    handle_exception(e); \
  } \
  catch (const timeout_exception& e) { \
    handle_exception(e); \
  } \
  catch(const std::overflow_error& e) { \
    handle_exception(e); \
  } \
  catch(const std::length_error& e) { \
    handle_exception(e); \
  } \
  catch (const std::bad_alloc& e) { \
    handle_exception(e); \
  } \
  catch (const std::exception& e) { \
    handle_exception(e); \
  } \
  catch (...) { \
    handle_exception(); \
  } \
  return PROLOG_FAILURE

#if 0
Prolog_term_ref
variable_term(dimension_type varid) {
  Prolog_term_ref v = Prolog_new_term_ref();
  Prolog_put_ulong(v, varid);
  Prolog_term_ref t = Prolog_new_term_ref();
  Prolog_construct_compound(t, a_dollar_VAR, v);
  return t;
}

unsigned int
get_unsigned_int(long n) {
  if (n >= 0 && static_cast<unsigned long>(n) <= UINT_MAX)
    return n;
  else {
    Prolog_term_ref n_term = Prolog_new_term_ref();
    Prolog_put_long(n_term, n);
    throw not_unsigned_integer(n_term, "get_unsigned_int");
  }
}
#endif

template <typename U>
U
term_to_unsigned(Prolog_term_ref t, const char* where) {
  using namespace Parma_Polyhedra_Library;
  using namespace Parma_Polyhedra_Library::Prolog_Interfaces;
  if (!Prolog_is_integer(t))
    throw not_unsigned_integer(t, where);

  U d = 0;
  long l;
  if (Prolog_get_long(t, &l))
    if (l < 0)
      throw not_unsigned_integer(t, where);
    else if (static_cast<unsigned long>(l) > std::numeric_limits<U>::max())
      throw Prolog_unsigned_out_of_range(t, where,
					 std::numeric_limits<U>::max());
    else
      d = l;
  else {
    TEMP_INTEGER(v);
    Prolog_get_Coefficient(t, v);
    if (v < 0)
      throw not_unsigned_integer(t, where);
    if (assign_r(d, raw_value(v), ROUND_NOT_NEEDED) != V_EQ)
      throw Prolog_unsigned_out_of_range(t, where,
					 std::numeric_limits<U>::max());
  }
  return d;
}

Prolog_atom
term_to_universe_or_empty(Prolog_term_ref t, const char* where);

Prolog_term_ref
interval_term(const Parma_Polyhedra_Library::Rational_Box::interval_type& i);

Prolog_atom
term_to_complexity_class(Prolog_term_ref t, const char* where);

template <typename T>
T*
term_to_handle(Prolog_term_ref t, const char* where) {
  if (Prolog_is_address(t)) {
    void* p;
    if (Prolog_get_address(t, &p))
      return static_cast<T*>(p);
  }
  throw ppl_handle_mismatch(t, where);
}

enum Boundary_Kind {
  LOWER_BOUNDARY,
  UPPER_BOUNDARY
};

bool
term_to_boundary(Prolog_term_ref t_b, Boundary_Kind kind,
		 bool& finite, bool& closed,
		 Parma_Polyhedra_Library::Coefficient& n, Parma_Polyhedra_Library::Coefficient& d);

Parma_Polyhedra_Library::Relation_Symbol
term_to_relation_symbol(Prolog_term_ref t_r, const char* where);

Parma_Polyhedra_Library::Coefficient
integer_term_to_Coefficient(Prolog_term_ref t);

Prolog_term_ref
Coefficient_to_integer_term(const Parma_Polyhedra_Library::Coefficient& n);

bool
unify_long(Prolog_term_ref t, long l);

bool
unify_ulong(Prolog_term_ref t, unsigned long l);

Parma_Polyhedra_Library::Linear_Expression
build_linear_expression(Prolog_term_ref t, const char* where);

Parma_Polyhedra_Library::Constraint
build_constraint(Prolog_term_ref t, const char* where);

Parma_Polyhedra_Library::Congruence
build_congruence(Prolog_term_ref t, const char* where);

Parma_Polyhedra_Library::Generator
build_generator(Prolog_term_ref t, const char* where);

Parma_Polyhedra_Library::Grid_Generator
build_grid_generator(Prolog_term_ref t, const char* where);

Prolog_term_ref
get_linear_expression(const Parma_Polyhedra_Library::Linear_Expression& le);

Prolog_term_ref
constraint_term(const Parma_Polyhedra_Library::Constraint& c);

Prolog_term_ref
congruence_term(const Parma_Polyhedra_Library::Congruence& cg);

Prolog_term_ref
generator_term(const Parma_Polyhedra_Library::Generator& g);

Prolog_term_ref
grid_generator_term(const Parma_Polyhedra_Library::Grid_Generator& g);

Parma_Polyhedra_Library::Variable
term_to_Variable(Prolog_term_ref t, const char* where);

Parma_Polyhedra_Library::Coefficient
term_to_Coefficient(Prolog_term_ref t, const char* where);

Prolog_atom
term_to_optimization_mode(Prolog_term_ref t, const char* where);

void
check_nil_terminating(Prolog_term_ref t, const char* where);

} // namespace Prolog_Interfaces

} // namespace Parma_Polyhedra_Library

extern "C" Prolog_foreign_return_type
ppl_version_major(Prolog_term_ref t_v);

extern "C" Prolog_foreign_return_type
ppl_version_minor(Prolog_term_ref t_v);

extern "C" Prolog_foreign_return_type
ppl_version_revision(Prolog_term_ref t_v);

extern "C" Prolog_foreign_return_type
ppl_version_beta(Prolog_term_ref t_v);

extern "C" Prolog_foreign_return_type
ppl_version(Prolog_term_ref t_v);

extern "C" Prolog_foreign_return_type
ppl_banner(Prolog_term_ref t_b);

extern "C" Prolog_foreign_return_type
ppl_max_space_dimension(Prolog_term_ref t_msd);

extern "C" Prolog_foreign_return_type
ppl_initialize();

extern "C" Prolog_foreign_return_type
ppl_finalize();

extern "C" Prolog_foreign_return_type
ppl_set_timeout_exception_atom(Prolog_term_ref t_tea);

extern "C" Prolog_foreign_return_type
ppl_timeout_exception_atom(Prolog_term_ref t);

extern "C" Prolog_foreign_return_type
ppl_set_timeout(Prolog_term_ref t_time);

extern "C" Prolog_foreign_return_type
ppl_reset_timeout();

extern "C" Prolog_foreign_return_type
ppl_Coefficient_is_bounded();

extern "C" Prolog_foreign_return_type
ppl_Coefficient_min(Prolog_term_ref t_min);

extern "C" Prolog_foreign_return_type
ppl_Coefficient_max(Prolog_term_ref t_max);

extern "C" Prolog_foreign_return_type
ppl_new_MIP_Problem_from_space_dimension
(Prolog_term_ref t_nd, Prolog_term_ref t_mip);

extern "C" Prolog_foreign_return_type
ppl_new_MIP_Problem(Prolog_term_ref t_nd,
		    Prolog_term_ref t_clist,
		    Prolog_term_ref t_le_expr,
		    Prolog_term_ref t_opt,
		    Prolog_term_ref t_mip);

extern "C" Prolog_foreign_return_type
ppl_new_MIP_Problem_from_MIP_Problem(Prolog_term_ref t_mip_source,
				     Prolog_term_ref t_mip);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_swap(Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

extern "C" Prolog_foreign_return_type
ppl_delete_MIP_Problem(Prolog_term_ref t_mip);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_space_dimension(Prolog_term_ref t_mip, Prolog_term_ref t_sd);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_integer_space_dimensions(Prolog_term_ref t_mip,
					 Prolog_term_ref t_vlist);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_constraints(Prolog_term_ref t_mip,
			    Prolog_term_ref t_clist);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_objective_function(Prolog_term_ref t_mip,
				   Prolog_term_ref t_le_expr);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_optimization_mode(Prolog_term_ref t_mip,
				  Prolog_term_ref t_opt);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_clear(Prolog_term_ref t_mip);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_add_space_dimensions_and_embed
(Prolog_term_ref t_mip, Prolog_term_ref t_nnd);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_add_to_integer_space_dimensions(Prolog_term_ref t_mip,
						Prolog_term_ref t_vlist);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_add_constraint(Prolog_term_ref t_mip, Prolog_term_ref t_c);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_add_constraints(Prolog_term_ref t_mip,
				Prolog_term_ref t_clist);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_set_objective_function(Prolog_term_ref t_mip,
				       Prolog_term_ref t_le_expr);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_set_optimization_mode(Prolog_term_ref t_mip,
				      Prolog_term_ref t_opt);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_is_satisfiable(Prolog_term_ref t_mip);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_solve(Prolog_term_ref t_mip, Prolog_term_ref t_status);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_feasible_point(Prolog_term_ref t_mip,
			       Prolog_term_ref t_g);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_optimizing_point(Prolog_term_ref t_mip,
				 Prolog_term_ref t_g);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_optimal_value(Prolog_term_ref t_mip,
			      Prolog_term_ref t_n,
			      Prolog_term_ref t_d);

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_evaluate_objective_function(Prolog_term_ref t_mip,
					    Prolog_term_ref t_g,
					    Prolog_term_ref t_n,
					    Prolog_term_ref t_d);

using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::Prolog_Interfaces;

extern "C" Prolog_foreign_return_type
ppl_MIP_Problem_OK(Prolog_term_ref t_mip);

class PFunc {
private:
  std::set<dimension_type> codomain;
  std::vector<dimension_type> vec;

public:
  PFunc() {
  }

  bool has_empty_codomain() const {
    return codomain.empty();
  }

  dimension_type max_in_codomain() const {
    if (codomain.empty())
      throw unknown_interface_error("PFunc::max_in_codomain()");
    return *codomain.rbegin();
  }

  bool maps(dimension_type i, dimension_type& j) const {
    if (i >= vec.size())
      return false;
    dimension_type vec_i = vec[i];
    if (vec_i == not_a_dimension())
      return false;
    j = vec_i;
    return true;
  }

  bool insert(dimension_type i, dimension_type j) {
    std::pair<std::set<dimension_type>::iterator, bool> s
      = codomain.insert(j);
    if (!s.second)
      // *this is not injective!
      return false;
    if (i > vec.size())
      vec.insert(vec.end(), i - vec.size(), not_a_dimension());
    if (i == vec.size()) {
      vec.insert(vec.end(), j);
      return true;
    }
    dimension_type& vec_i = vec[i];
    if (vec_i != not_a_dimension())
      // Already mapped: *this is not a function!
      return false;
    vec_i = j;
    return true;
  }
};

#include "ppl_prolog_common.inlines.hh"

#endif // !defined(PPL_ppl_prolog_common_defs_hh)
