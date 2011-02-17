/* Congruence_System class implementation (non-inline functions).
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2011 BUGSENG srl (http://bugseng.com)

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

#include <ppl-config.h>

#include "Congruence_System.defs.hh"
#include "Congruence_System.inlines.hh"
#include "Constraint_System.defs.hh"
#include "Constraint_System.inlines.hh"
#include "Congruence.defs.hh"
#include "Grid_Generator.defs.hh"
#include "Scalar_Products.defs.hh"
#include "assert.hh"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace PPL = Parma_Polyhedra_Library;

PPL::Congruence_System::Congruence_System(const Constraint_System& cs)
  : rows(),
    num_columns_(cs.space_dimension() + 2) {
  for (Constraint_System::const_iterator i = cs.begin(),
	 cs_end = cs.end(); i != cs_end; ++i)
    if (i->is_equality())
      insert(*i);
}

void
PPL::Congruence_System
::permute_columns(const std::vector<dimension_type>& cycles) {
  PPL_DIRTY_TEMP_COEFFICIENT(tmp);
  const dimension_type n = cycles.size();
  PPL_ASSERT(cycles[n - 1] == 0);
  for (dimension_type k = num_rows(); k-- > 0; ) {
    Congruence& rows_k = rows[k];
    for (dimension_type i = 0, j = 0; i < n; i = ++j) {
      // Make `j' be the index of the next cycle terminator.
      while (cycles[j] != 0)
        ++j;
      // Cycles of length less than 2 are not allowed.
      PPL_ASSERT(j - i >= 2);
      if (j - i == 2)
        // For cycles of length 2 no temporary is needed, just a swap.
        rows_k.swap(cycles[i], cycles[i + 1]);
      else {
        // Longer cycles need a temporary.
        tmp = rows_k.coefficient(Variable(cycles[j - 1] - 1));
        for (dimension_type l = (j - 1); l > i; --l)
          rows_k.swap(cycles[l-1], cycles[l]);
        if (tmp == 0)
          rows_k.set_coefficient(Variable(cycles[i] - 1), 0);
        else
          rows_k.set_coefficient(Variable(cycles[i] - 1), tmp);
      }
    }
  }
}

bool
PPL::Congruence_System
::increase_space_dimension(const dimension_type new_space_dim) {
  PPL_ASSERT(space_dimension() <= new_space_dim);

  const dimension_type diff = new_space_dim - space_dimension();

  if (diff != 0) {
    num_columns_ += diff;
    for (dimension_type i = num_rows(); i-- > 0; )
      rows[i].add_space_dimensions(diff);
  }

  PPL_ASSERT(OK());
  return true;
}

void
PPL::Congruence_System::swap_columns(dimension_type i, dimension_type j) {
  PPL_ASSERT(i < num_columns());
  PPL_ASSERT(j < num_columns());
  for (dimension_type k = num_rows(); k-- > 0; )
    rows[k].swap(i, j);
}

void
PPL::Congruence_System::insert_verbatim(const Congruence& cg) {
  const dimension_type old_num_columns = num_columns();
  const dimension_type cg_size = cg.space_dimension() + 2;

  if (cg_size >= old_num_columns) {
    // Resize the system, if necessary.
    increase_space_dimension(cg.space_dimension());
    rows.push_back(cg);
  } else {
    // Create a resized copy of `cg'.
    Congruence rc(cg, space_dimension());
    rows.resize(num_rows() + 1);
    std::swap(rc, rows.back());
  }

  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::insert(const Constraint& c) {
  if (c.space_dimension() > space_dimension())
    increase_space_dimension(c.space_dimension());
  Congruence cg(c, space_dimension());
  cg.strong_normalize();
  rows.resize(num_rows() + 1);
  std::swap(cg, rows.back());

  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::recycling_insert(Congruence_System& cgs) {
  const dimension_type old_num_rows = num_rows();
  const dimension_type cgs_num_rows = cgs.num_rows();
  if (space_dimension() < cgs.space_dimension())
    increase_space_dimension(cgs.space_dimension());
  rows.resize(old_num_rows + cgs_num_rows);
  for (dimension_type i = cgs_num_rows; i-- > 0; ) {
    cgs[i].set_space_dimension(space_dimension());
    std::swap(cgs[i], rows[old_num_rows + i]);
  }
  cgs.clear();

  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::insert(const Congruence_System& y) {
  Congruence_System& x = *this;

  const dimension_type x_num_rows = x.num_rows();
  const dimension_type y_num_rows = y.num_rows();

  // Grow to the required size.
  if (space_dimension() < y.space_dimension())
    increase_space_dimension(y.space_dimension());

  rows.resize(rows.size() + y_num_rows);

  // Copy the rows of `y', with the new space dimension.
  for (dimension_type i = y_num_rows; i-- > 0; ) {
    Congruence copy(y[i], space_dimension());
    std::swap(copy, x[x_num_rows+i]);
  }
  PPL_ASSERT(OK());
}

void
PPL::Congruence_System::normalize_moduli() {
  dimension_type row = num_rows();
  if (row > 0) {
    // Calculate the LCM of all the moduli.
    PPL_DIRTY_TEMP_COEFFICIENT(lcm);
    // Find last proper congruence.
    while (true) {
      lcm = operator[](--row).modulus();
      if (lcm > 0)
	break;
      if (row == 0)
	// All rows are equalities.
	return;
    }
    while (row > 0) {
      const Coefficient& modulus = operator[](--row).modulus();
      if (modulus > 0)
	lcm_assign(lcm, lcm, modulus);
    }

    // Represent every row using the LCM as the modulus.
    PPL_DIRTY_TEMP_COEFFICIENT(factor);
    dimension_type row_size = operator[](0).size();
    for (row = num_rows(); row-- > 0; ) {
      const Coefficient& modulus = operator[](row).modulus();
      if (modulus <= 0 || modulus == lcm)
	continue;
      exact_div_assign(factor, lcm, modulus);
      for (dimension_type col = row_size; col-- > 0; )
	operator[](row)[col] *= factor;
      operator[](row)[row_size-1] = lcm;
    }
  }
  PPL_ASSERT(OK());
}

bool
PPL::Congruence_System::is_equal_to(const Congruence_System& cgs) const {
  if (num_rows() != cgs.num_rows())
    return false;

  PPL_ASSERT(num_columns() >= 2);
  PPL_ASSERT(cgs.num_columns() >= 2);

  // FIXME: What happens when num_columns() != cgs.num_columns()?

  for (dimension_type row = cgs.num_rows(); row-- > 0; ) {
    for (dimension_type col = cgs.num_columns() - 2; col-- > 0; )
      if ((*this)[row].coefficient(Variable(col))
          != cgs[row].coefficient(Variable(col)))
        return false;

    if ((*this)[row].inhomogeneous_term() != cgs[row].inhomogeneous_term())
      return false;
    if ((*this)[row].modulus() != cgs[row].modulus())
      return false;
  }
  return true;
}

bool
PPL::Congruence_System::has_linear_equalities() const {
  const Congruence_System& cgs = *this;
  for (dimension_type i = cgs.num_rows(); i-- > 0; )
    if (cgs[i].modulus() == 0)
      return true;
  return false;
}

void
PPL::Congruence_System::const_iterator::skip_forward() {
  const Swapping_Vector<Congruence>::const_iterator csp_end = csp->end();
  while (i != csp_end && (*this)->is_tautological())
    ++i;
}

PPL::dimension_type
PPL::Congruence_System::num_equalities() const {
  const Congruence_System& cgs = *this;
  dimension_type n = 0;
  for (dimension_type i = num_rows(); i-- > 0 ; )
    if (cgs[i].is_equality())
      ++n;
  return n;
}

PPL::dimension_type
PPL::Congruence_System::num_proper_congruences() const {
  const Congruence_System& cgs = *this;
  dimension_type n = 0;
  for (dimension_type i = num_rows(); i-- > 0 ; ) {
    const Congruence& cg = cgs[i];
    if (cg.is_proper_congruence())
      ++n;
  }
  return n;
}

bool
PPL::Congruence_System::
satisfies_all_congruences(const Grid_Generator& g) const {
  PPL_ASSERT(g.space_dimension() <= space_dimension());

  const Congruence_System& cgs = *this;
  PPL_DIRTY_TEMP_COEFFICIENT(sp);
  if (g.is_line())
    for (dimension_type i = cgs.num_rows(); i-- > 0; ) {
      const Congruence& cg = cgs[i];
      Scalar_Products::assign(sp, g, cg);
      if (sp != 0)
	return false;
    }
  else {
    const Coefficient& divisor = g.divisor();
    for (dimension_type i = cgs.num_rows(); i-- > 0; ) {
      const Congruence& cg = cgs[i];
      Scalar_Products::assign(sp, g, cg);
      if (cg.is_equality()) {
	if (sp != 0)
	  return false;
      }
      else if (sp % (cg.modulus() * divisor) != 0)
	return false;
    }
  }
  return true;
}

bool
PPL::Congruence_System::has_a_free_dimension() const {
  // Search for a dimension that is free of any congruence or equality
  // constraint.  Assumes a minimized system.
  dimension_type space_dim = space_dimension();
  std::vector<bool> free_dim(space_dim, true);
  dimension_type free_dims = space_dim;
  for (dimension_type row = num_rows(); row-- > 0; ) {
    const Congruence& cg = operator[](row);
    for (dimension_type dim = space_dim; dim-- > 0; )
      if (free_dim[dim] && cg.coefficient(Variable(dim)) != 0) {
	if (--free_dims == 0) {
	  // All dimensions are constrained.
#ifndef NDEBUG
	  free_dim[dim] = false;
	  // Check that there are free_dims dimensions marked free
	  // in free_dim.
	  dimension_type count = 0;
	  for (dimension_type i = space_dim; i-- > 0; )
	    count += free_dim[i];
	  PPL_ASSERT(count == free_dims);
#endif
	  return true;
	}
	free_dim[dim] = false;
      }
  }
  // At least one dimension is free of constraint.
  return false;
}

void
PPL::Congruence_System::
affine_preimage(dimension_type v,
		const Linear_Expression& expr,
		Coefficient_traits::const_reference denominator) {
  // `v' is the index of a column corresponding to a "user" variable
  // (i.e., it cannot be the inhomogeneous term).
  PPL_ASSERT(v > 0 && v <= space_dimension());
  PPL_ASSERT(expr.space_dimension() <= space_dimension());
  PPL_ASSERT(denominator > 0);

  const dimension_type num_columns = this->num_columns();
  const dimension_type num_rows = this->num_rows();
  const dimension_type expr_size = expr.size();
  const bool not_invertible = (v >= expr_size || expr[v] == 0);
  Congruence_System& x = *this;

  if (denominator == 1)
    // Optimized computation only considering columns having indexes <
    // expr_size.
    for (dimension_type i = num_rows; i-- > 0; ) {
      Congruence& row = x[i];
      Coefficient& row_v = row[v];
      if (row_v != 0) {
	for (dimension_type j = expr_size; j-- > 0; )
	  if (j != v)
	    // row[j] = row[j] + row_v * expr[j]
	    add_mul_assign(row[j], row_v, expr[j]);
	if (not_invertible)
	  row_v = 0;
	else
	  row_v *= expr[v];
      }
    }
  else
    for (dimension_type i = num_rows; i-- > 0; ) {
      Congruence& row = x[i];
      Coefficient& row_v = row[v];
      if (row_v != 0) {
	for (dimension_type j = num_columns; j-- > 0; )
	  if (j != v) {
	    Coefficient& row_j = row[j];
	    row_j *= denominator;
	    if (j < expr_size)
	      add_mul_assign(row_j, row_v, expr[j]);
	  }
	if (not_invertible)
	  row_v = 0;
	else
	  row_v *= expr[v];
      }
    }
}

void
PPL::Congruence_System::ascii_dump(std::ostream& s) const {
  const Congruence_System& x = *this;
  dimension_type x_num_rows = x.num_rows();
  dimension_type x_num_columns = x.num_columns();
  s << x_num_rows << " x " << x_num_columns
    << std::endl;
  if (x_num_rows && x_num_columns)
    for (dimension_type i = 0; i < x_num_rows; ++i)
      x[i].ascii_dump(s);
}

PPL_OUTPUT_DEFINITIONS(Congruence_System)

bool
PPL::Congruence_System::ascii_load(std::istream& s) {
  std::string str;
  dimension_type num_rows;
  dimension_type num_columns;
  if (!(s >> num_rows))
    return false;
  if (!(s >> str) || str != "x")
    return false;
  if (!(s >> num_columns))
    return false;
  resize_no_copy(num_rows, num_columns);

  Congruence_System& x = *this;
  for (dimension_type i = 0; i < x.num_rows(); ++i)
    if (!x[i].ascii_load(s))
      return false;

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

const PPL::Congruence_System* PPL::Congruence_System::zero_dim_empty_p = 0;

void
PPL::Congruence_System::initialize() {
  PPL_ASSERT(zero_dim_empty_p == 0);
  zero_dim_empty_p
    = new Congruence_System(Congruence::zero_dim_false());
}

void
PPL::Congruence_System::finalize() {
  PPL_ASSERT(zero_dim_empty_p != 0);
  delete zero_dim_empty_p;
  zero_dim_empty_p = 0;
}

bool
PPL::Congruence_System::OK() const {
  // All rows must have space dimension `space_dimension()'.
  for (dimension_type i = num_rows(); i-- > 0; )
    if (rows[i].space_dimension() != space_dimension())
      return false;

  if (num_rows() != 0) {
    if (num_columns() < 2) {
#ifndef NDEBUG
      std::cerr << "Congruence_System has rows and fewer than two columns."
		<< std::endl;
#endif
      return false;
    }
  }

  // Checking each congruence in the system.
  const Congruence_System& x = *this;
  for (dimension_type i = num_rows(); i-- > 0; ) {
    const Congruence& cg = x[i];
    if (!cg.OK())
      return false;
  }

  // All checks passed.
  return true;
}

/*! \relates Parma_Polyhedra_Library::Congruence_System */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Congruence_System& cgs) {
  Congruence_System::const_iterator i = cgs.begin();
  const Congruence_System::const_iterator cgs_end = cgs.end();
  if (i == cgs_end)
    return s << "true";
  while (true) {
    Congruence cg = *i++;
    cg.strong_normalize();
    s << cg;
    if (i == cgs_end)
      return s;
    s << ", ";
  }
}

/*! \relates Parma_Polyhedra_Library::Congruence_System */
bool
PPL::operator==(const Congruence_System& x, const Congruence_System& y) {
  if (x.num_columns() == y.num_columns()) {
    dimension_type num_rows = x.num_rows();
    if (num_rows == y.num_rows()) {
      while (num_rows--) {
	if (x[num_rows] == y[num_rows])
	  continue;
	return false;
      }
      return true;
    }
  }
  return false;
}

void
PPL::Congruence_System::add_unit_rows_and_columns(dimension_type dims) {
  PPL_ASSERT(num_columns() > 0);
  dimension_type old_num_rows = num_rows();
  increase_space_dimension(space_dimension() + dims);

  rows.resize(rows.size() + dims);

  // Swap the added rows to the front of the vector.
  for (dimension_type row = old_num_rows; row-- > 0; )
    std::swap(operator[](row), operator[](row + dims));

  PPL_ASSERT(num_columns() >= 2);
  const dimension_type col = num_columns() - 2;
  // Set the space dimension and the diagonal element of each added row.
  for (dimension_type row = dims; row-- > 0; ) {
    rows[row].set_space_dimension(space_dimension());
    PPL_ASSERT(col >= row + 1);
    rows[row].set_coefficient(Variable(col - row - 1), Coefficient_one());
  }
}

void
PPL::Congruence_System::concatenate(const Congruence_System& const_cgs) {
  // TODO: this implementation is just an executable specification.
  Congruence_System cgs = const_cgs;

  dimension_type added_rows = cgs.num_rows();
  dimension_type added_columns = cgs.space_dimension();

  dimension_type old_num_rows = num_rows();
  dimension_type old_space_dim = space_dimension();

  increase_space_dimension(space_dimension() + added_columns);

  rows.resize(rows.size() + added_rows);

  // Move the congruences into *this from `cgs', shifting the
  // coefficients along into the appropriate columns.
  for (dimension_type i = added_rows; i-- > 0; ) {
    Congruence& cg_old = cgs[i];
    Congruence& cg_new = operator[](old_num_rows + i);
    cg_old.shift_coefficients(old_space_dim, 0);
    std::swap(cg_old, cg_new);
  }
}

void
PPL::Congruence_System
::remove_higher_space_dimensions(const dimension_type new_dimension) {
  dimension_type space_dim = space_dimension();

  PPL_ASSERT(new_dimension <= space_dim);

  // The removal of no dimensions from any system is a no-op.  Note
  // that this case also captures the only legal removal of dimensions
  // from a system in a 0-dim space.
  if (new_dimension == space_dim)
    return;

  // Swap the modulus column into the column that will become the last
  // column.
  swap_columns(new_dimension + 1, space_dim + 1);

  remove_trailing_columns(space_dim - new_dimension);
  PPL_ASSERT(OK());
}
