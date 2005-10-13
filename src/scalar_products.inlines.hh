/* Scalar_Products class implementation (inline functions).
   Copyright (C) 2001-2005 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
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

#ifndef PPL_scalar_products_inlines_hh
#define PPL_scalar_products_inlines_hh 1

#include "Linear_Row.defs.hh"
#include "Linear_Expression.defs.hh"
#include "Constraint.defs.hh"
#include "Generator.defs.hh"

namespace Parma_Polyhedra_Library {

inline int
Scalar_Products::sign(const Linear_Row& x, const Linear_Row& y) {
  TEMP_INTEGER(z);
  assign(z, x, y);
  return sgn(z);
}

inline int
Scalar_Products::reduced_sign(const Linear_Row& x, const Linear_Row& y) {
  TEMP_INTEGER(z);
  reduced_assign(z, x, y);
  return sgn(z);
}

inline int
Scalar_Products::homogeneous_sign(const Linear_Row& x, const Linear_Row& y) {
  TEMP_INTEGER(z);
  homogeneous_assign(z, x, y);
  return sgn(z);
}

inline int
Scalar_Products::sign(const Constraint& c, const Generator& g) {
  return sign(static_cast<const Linear_Row&>(c),
	      static_cast<const Linear_Row&>(g));
}

inline int
Scalar_Products::sign(const Generator& g, const Constraint& c) {
  return sign(static_cast<const Linear_Row&>(g),
	      static_cast<const Linear_Row&>(c));
}

inline int
Scalar_Products::reduced_sign(const Constraint& c, const Generator& g) {
  return reduced_sign(static_cast<const Linear_Row&>(c),
		      static_cast<const Linear_Row&>(g));
}

inline int
Scalar_Products::reduced_sign(const Generator& g, const Constraint& c) {
  return reduced_sign(static_cast<const Linear_Row&>(g),
		      static_cast<const Linear_Row&>(c));
}

inline void
Scalar_Products::homogeneous_assign(Coefficient& z,
				    const Linear_Expression& e,
				    const Generator& g) {
  homogeneous_assign(z,
		     static_cast<const Linear_Row&>(e),
		     static_cast<const Linear_Row&>(g));
}

inline int
Scalar_Products::homogeneous_sign(const Linear_Expression& e,
				  const Generator& g) {
  return homogeneous_sign(static_cast<const Linear_Row&>(e),
			  static_cast<const Linear_Row&>(g));
}

inline
Topology_Adjusted_Scalar_Product_Sign
::Topology_Adjusted_Scalar_Product_Sign(const Constraint& c)
  : sps_fp(c.is_necessarily_closed()
	   ? static_cast<SPS_type>(&Scalar_Products::sign)
	   : static_cast<SPS_type>(&Scalar_Products::reduced_sign)) {
}

inline
Topology_Adjusted_Scalar_Product_Sign
::Topology_Adjusted_Scalar_Product_Sign(const Generator& g)
  : sps_fp(g.is_necessarily_closed()
	   ? static_cast<SPS_type>(&Scalar_Products::sign)
	   : static_cast<SPS_type>(&Scalar_Products::reduced_sign)) {
}

inline int
Topology_Adjusted_Scalar_Product_Sign::operator()(const Constraint& c,
						  const Generator& g) const {
  assert(c.space_dimension() <= g.space_dimension());
  assert(sps_fp == (c.is_necessarily_closed()
		    ? static_cast<SPS_type>(&Scalar_Products::sign)
		    : static_cast<SPS_type>(&Scalar_Products::reduced_sign)));
  return sps_fp(static_cast<const Linear_Row&>(c),
		static_cast<const Linear_Row&>(g));
}

inline int
Topology_Adjusted_Scalar_Product_Sign::operator()(const Generator& g,
						  const Constraint& c) const {
  assert(g.space_dimension() <= c.space_dimension());
  assert(sps_fp == (g.is_necessarily_closed()
		    ? static_cast<SPS_type>(&Scalar_Products::sign)
		    : static_cast<SPS_type>(&Scalar_Products::reduced_sign)));
  return sps_fp(static_cast<const Linear_Row&>(g),
		static_cast<const Linear_Row&>(c));
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_scalar_products_inlines_hh)
