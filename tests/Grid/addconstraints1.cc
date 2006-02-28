/* Test adding constraints to a grid.
   Copyright (C) 2001-2006 Roberto Bagnara <bagnara@cs.unipr.it>

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

#include "ppl_test.hh"

namespace {

// add_congruences_and_minimize(cs)
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);

  print_congruences(gr, "*** gr ***");

  Constraint_System cs;
  cs.insert(B == 0);
  cs.insert(A >= 0);
  cs.insert(C > 0);

  gr.add_congruences_and_minimize(cs);

  Grid known_gr(3);
  known_gr.add_congruence(B == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_congruences_and_minimize(cs) ***");

  return ok;
}

// add_constraints
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(B == 0);
  cs.insert(A >= 0);
  cs.insert(C > 0);

  Grid gr(3);

  print_congruences(gr, "*** gr ***");

  gr.add_constraints(cs);

  Grid known_gr(3);
  known_gr.add_congruence(B == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_constraints(cs) ***");

  return ok;
}

// add_constraints, resulting grid empty.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(B < 0);
  cs.insert(A >= 0);
  cs.insert(C > 0);

  Grid gr(3);

  print_congruences(gr, "*** gr ***");

  gr.add_constraints(cs);

  Grid known_gr(3);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_constraints(cs) ***");

  return ok;
}

// add_congruences(cs)
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(B < 0);
  cs.insert(B > 0);
  cs.insert(A == 0);
  cs.insert(C > 0);

  Grid gr(3);

  print_congruences(gr, "*** gr ***");

  gr.add_congruences(cs);

  Grid known_gr(3);
  known_gr.add_congruence(A == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_congruences(cs) ***");

  return ok;
}

// add_recycled_congruences(cs)
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(2*B == 3);
  cs.insert(A == 0);
  cs.insert(C > 0);

  Grid gr(3);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_congruences(cs);

  Grid known_gr(3);
  known_gr.add_congruence(A == 0);
  known_gr.add_congruence(2*B == 3);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_recycled_congruences(cs) ***");

  return ok;
}

// add_recycled_congruences_and_minimize(cs)
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(2*B >= 3);
  cs.insert(2*A == 7);
  cs.insert(C > 0);

  Grid gr(3);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_congruences_and_minimize(cs);

  Grid known_gr(3);
  known_gr.add_congruence(2*A == 7);

  bool ok = (gr == known_gr);

  print_congruences(gr,
      "*** gr.add_recycled_congruences_and_minimize(cs) ***");

  return ok;
}

// add_constraints_and_minimize(cs)

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(2*B >= 3);
  cs.insert(D == 0);
  cs.insert(2*A == C);

  Grid gr(4);

  print_congruences(gr, "*** gr ***");

  gr.add_constraints_and_minimize(cs);

  Grid known_gr(4);
  known_gr.add_congruence(2*A == C);
  known_gr.add_congruence(D == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_constraints_and_minimize(cs) ***");

  return ok;
}

// add_recycled_constraints
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(2*B > 2);
  cs.insert(2*D == 0);

  Grid gr(4);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_constraints(cs);

  Grid known_gr(4);
  known_gr.add_congruence(D == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_recycled_constraints(cs) ***");

  return ok;
}

// add_recycled_constraints_and_minimize
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(2*B > 6);
  cs.insert(2*C == 6*D);

  Grid gr(4);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_constraints_and_minimize(cs);

  Grid known_gr(4);
  known_gr.add_congruence(C == 3*D);

  bool ok = (gr == known_gr);

  print_congruences(gr,
      "*** gr.add_recycled_constraints_and_minimize(cs) ***");

  return ok;
}

// add_recycled_congruences(cs) -- space dimension exception
bool
test10() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B == 0);

  Grid gr(1);

  try {
    gr.add_recycled_congruences(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_congruences(cs) -- space dimension exception
bool
test11() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B == 0);

  Grid gr(1);

  try {
    gr.add_congruences(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_recycled_congruences_and_minimize(cs) -- space dimension
// exception
bool
test12() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B == 0);

  Grid gr(1);

  try {
    gr.add_recycled_congruences_and_minimize(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_congruences_and_minimize(cs) -- space dimension exception
bool
test13() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B == 0);

  Grid gr(1);

  try {
    gr.add_congruences_and_minimize(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_constraints(cs) -- space dimension exception
bool
test14() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B == 0);

  Grid gr(1);

  try {
    gr.add_constraints(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_constraints(cs) -- space dimension exception
bool
test15() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B == 0);

  Grid gr(1);

  try {
    gr.add_constraints(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_recycled_constraints(cs) -- space dimension exception
bool
test16() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B == 0);

  Grid gr(1);

  try {
    gr.add_recycled_constraints(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_recycled_constraints_and_minimize(cs) -- space dimension
// exception
bool
test17() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B == 0);

  Grid gr(1);

  try {
    gr.add_recycled_constraints_and_minimize(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

// add_constraints_and_minimize(cs) -- space dimension exception
bool
test18() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B == 0);

  Grid gr(1);

  try {
    gr.add_constraints_and_minimize(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
  }
  catch (...) {
    return false;
  }
  return true;
}

} // namespace

BEGIN_MAIN
  NEW_TEST(test01);
  NEW_TEST(test02);
  NEW_TEST(test03);
  NEW_TEST(test04);
  NEW_TEST(test05);
  NEW_TEST(test06);
  NEW_TEST(test07);
  NEW_TEST(test08);
  NEW_TEST(test09);
  NEW_TEST(test10);
  NEW_TEST(test11);
  NEW_TEST(test12);
  NEW_TEST(test13);
  NEW_TEST(test14);
  NEW_TEST(test15);
  NEW_TEST(test16);
  NEW_TEST(test17);
  NEW_TEST(test18);
END_MAIN
