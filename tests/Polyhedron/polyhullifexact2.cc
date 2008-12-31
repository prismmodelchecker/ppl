/* Test NNC_Polyhedron::upper_bound_assign_if_exact().
   Copyright (C) 2001-2009 Roberto Bagnara <bagnara@cs.unipr.it>

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

#include "ppl_test.hh"

namespace {

bool
test01() {
  Variable x(0);

  NNC_Polyhedron ph1(1, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x <= 1);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(1, UNIVERSE);
  ph2.add_constraint(x == 0);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(1, UNIVERSE);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 1);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x > 0);
  ph1.add_constraint(x <= 2);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 2);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y == 1);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x <= 2);
  ph1.add_constraint(y >= 0);
  ph1.add_constraint(y <= 2);
  ph1.add_constraint(x + y > 0);
  ph1.add_constraint(x + y < 4);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x == y);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(y >= 0);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(2, UNIVERSE);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);

  bool ok = ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}


bool
test04() {
  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2, UNIVERSE);
  ph1.add_constraint(x - y >= 0);
  ph1.add_constraint(x + y >= 0);
  ph1.add_constraint(4*x < 1);

  print_constraints(ph1, "*** ph1 ***");

  NNC_Polyhedron ph2(2, UNIVERSE);
  ph2.add_constraint(x - y >= 0);
  ph2.add_constraint(x + y >= 0);
  ph2.add_constraint(4*x > 1);

  print_constraints(ph2, "*** ph2 ***");

  NNC_Polyhedron known_result(ph1);

  bool ok = !ph1.upper_bound_assign_if_exact(ph2);
  ok &= (ph1 == known_result);

  print_constraints(ph1, "*** ph1.upper_bound_assign_if_exact(ph2) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
