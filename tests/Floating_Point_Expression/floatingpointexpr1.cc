/* Testing class Floating_Point_Expression ad its derivate classes.
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

typedef Linear_Form<fl_r_oc> Float_Interval_Linear_Form;
typedef Linear_Form<db_r_oc> Double_Interval_Linear_Form;

typedef Division_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_single> div_fpess;
typedef Division_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_double> div_fpesd;
typedef Difference_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_single> dif_fpess;
typedef Difference_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_double> dif_fpesd;
typedef Multiplication_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_single> mul_fpess;
typedef Multiplication_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_double> mul_fpesd;
typedef Sum_Floating_Point_Expression<fl_r_oc,
                                      float_ieee754_single> sum_fpess;
typedef Sum_Floating_Point_Expression<fl_r_oc,
                                      float_ieee754_double> sum_fpesd;
typedef Constant_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_single> con_fpess;
typedef Constant_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_double> con_fpesd;
typedef Variable_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_single> var_fpess;
typedef Variable_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_double> var_fpesd;
typedef Opposite_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_single> opp_fpess;
typedef Opposite_Floating_Point_Expression<fl_r_oc,
                                           float_ieee754_double> opp_fpesd;
typedef Division_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_single> div_fpeds;
typedef Division_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_double> div_fpedd;
typedef Difference_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_single> dif_fpeds;
typedef Difference_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_double> dif_fpedd;
typedef Multiplication_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_single> mul_fpeds;
typedef Multiplication_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_double> mul_fpedd;
typedef Sum_Floating_Point_Expression<db_r_oc,
                                      float_ieee754_single> sum_fpeds;
typedef Sum_Floating_Point_Expression<db_r_oc,
                                      float_ieee754_double> sum_fpedd;
typedef Constant_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_single> con_fpeds;
typedef Constant_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_double> con_fpedd;
typedef Variable_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_single> var_fpeds;
typedef Variable_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_double> var_fpedd;
typedef Opposite_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_single> opp_fpeds;
typedef Opposite_Floating_Point_Expression<db_r_oc,
                                           float_ieee754_double> opp_fpedd;
typedef Floating_Point_Expression<fl_r_oc,
                float_ieee754_single>::FP_Linear_Form_Abstract_Store lsstr;
typedef Floating_Point_Expression<db_r_oc,
                float_ieee754_single>::FP_Linear_Form_Abstract_Store ldstr;
typedef Floating_Point_Expression<fl_r_oc,
                float_ieee754_double>::FP_Linear_Form_Abstract_Store lsdtr;
typedef Floating_Point_Expression<db_r_oc,
                float_ieee754_double>::FP_Linear_Form_Abstract_Store lddtr;

namespace {

using namespace Parma_Polyhedra_Library::IO_Operators;

// Tests absolute errors.
bool
test01() {
  nout << std::numeric_limits<float>::denorm_min() << endl;
  nout << div_fpess::absolute_error << endl;
  nout << div_fpesd::absolute_error << endl;
  nout << div_fpeds::absolute_error << endl;
  nout << div_fpedd::absolute_error << endl;

  if (div_fpess::absolute_error != std::numeric_limits<float>::denorm_min())
    return false;

  if (div_fpesd::absolute_error != std::numeric_limits<float>::denorm_min())
    return false;

  if (div_fpeds::absolute_error != std::numeric_limits<float>::denorm_min())
    return false;

  if (div_fpedd::absolute_error != std::numeric_limits<double>::denorm_min())
    return false;

  return true;
}

// Tests division by zero.
bool
test02() {
  con_fpess* num = new con_fpess(3, 5);
  con_fpess* den = new con_fpess(-1, 1);
  div_fpess div(num, den);
  try {
    Float_Interval_Linear_Form result;
    div.linearize(Box<fl_r_oc>(0), lsstr(), result);
  }
  catch (Linearization_Failed e) {
    nout << "*** Linearization failed due to division by zero. ***" << endl;
    return true;
  }
  return false;
}

// Tests multiplication by zero.
bool
test03() {
  Box<fl_r_oc> store_fl(2);
  store_fl.set_interval(Variable(0), fl_r_oc(0));
  store_fl.set_interval(Variable(1), fl_r_oc(10));
  con_fpess* con_fl = new con_fpess(5, 6);
  var_fpess* var0_fl = new var_fpess(0);
  var_fpess* var1_fl = new var_fpess(1);
  dif_fpess* dif_fl = new dif_fpess(var1_fl, con_fl);
  mul_fpess mul_fl(dif_fl, var0_fl);
  Float_Interval_Linear_Form result_fl;
  mul_fl.linearize(store_fl, lsstr(), result_fl);
  fl_r_oc kr_fl(-std::numeric_limits<float>::denorm_min());
  kr_fl.join_assign(std::numeric_limits<float>::denorm_min());
  Float_Interval_Linear_Form known_result_fl(kr_fl);

  nout << "*** known_result_fl ***" << endl
       << known_result_fl << endl;
  bool ok_fl = (result_fl == known_result_fl);

  Box<db_r_oc> store_db(2);
  store_db.set_interval(Variable(0), db_r_oc(0));
  store_db.set_interval(Variable(1), db_r_oc(4));
  con_fpedd* con_db = new con_fpedd(5, 6);
  var_fpedd* var0_db = new var_fpedd(0);
  var_fpedd* var1_db = new var_fpedd(1);
  sum_fpedd* sum_db = new sum_fpedd(con_db, var1_db);
  mul_fpedd mul_db(var0_db, sum_db);
  Double_Interval_Linear_Form result_db;
  mul_db.linearize(store_db, ldstr(), result_db);

  db_r_oc kr_db(-std::numeric_limits<double>::denorm_min());
  kr_db.join_assign(std::numeric_limits<double>::denorm_min());
  Double_Interval_Linear_Form known_result_db(kr_db);

  nout << "*** known_result_db ***" << endl
       << known_result_db << endl;
  bool ok_db = (result_db == known_result_db);

  return ok_fl && ok_db;
}

// Tests the linearization of variables in a given linear form abstract store.
bool
test04() {
  lsdtr store_fl;
  Variable A(0);
  Float_Interval_Linear_Form known_result_fl = Float_Interval_Linear_Form(A);
  store_fl[0] = known_result_fl;
  var_fpesd var_fl(0);
  Float_Interval_Linear_Form result_fl;
  var_fl.linearize(Box<fl_r_oc>(0), store_fl, result_fl);

  nout << "*** known_result_fl ***" << endl
       << known_result_fl << endl;
  bool ok_fl = (result_fl == known_result_fl);

  lddtr store_db;
  Double_Interval_Linear_Form known_result_db =
  Double_Interval_Linear_Form(A);
  store_db[0] = known_result_db;
  var_fpedd var_db(0);
  Double_Interval_Linear_Form result_db;
  var_db.linearize(Box<db_r_oc>(0), store_db, result_db);

  nout << "*** known_result_db ***" << endl
       << known_result_db << endl;
  bool ok_db = (result_db == known_result_db);

  return ok_fl && ok_db;
}

// Tests the linearization of A + B.
bool
test05() {
  db_r_oc tmp;
  Box<db_r_oc> store(2);
  store.set_interval(Variable(0), tmp);
  store.set_interval(Variable(1), tmp);
  var_fpeds* var0 = new var_fpeds(0);
  var_fpeds* var1 = new var_fpeds(1);
  sum_fpeds sum(var0, var1);
  Double_Interval_Linear_Form result;
  sum.linearize(store, ldstr(), result);

  Variable A(0);
  Variable B(1);
  Double_Interval_Linear_Form known_result = Double_Interval_Linear_Form(A);
  float exp = pow(2, -23);
  tmp = db_r_oc(1 - exp);
  tmp.join_assign(1 + exp);
  known_result *= tmp;
  known_result += tmp * Linear_Form<db_r_oc>(B);
  tmp = db_r_oc(-std::numeric_limits<float>::denorm_min());
  tmp.join_assign(std::numeric_limits<float>::denorm_min());
  known_result += tmp;

  nout << "*** known_result ***" << endl
       << known_result << endl;
  return result == known_result;
}

// Tests the linearization of A - B.
bool
test06() {
  fl_r_oc tmp;
  Box<fl_r_oc> store(2);
  store.set_interval(Variable(0), tmp);
  store.set_interval(Variable(1), tmp);
  var_fpess* var0 = new var_fpess(0);
  var_fpess* var1 = new var_fpess(1);
  dif_fpess dif(var0, var1);
  Float_Interval_Linear_Form result;
  dif.linearize(store, lsstr(), result);

  Variable A(0);
  Variable B(1);
  Float_Interval_Linear_Form known_result = Float_Interval_Linear_Form(A);
  float exp = pow(2, -23);
  tmp = fl_r_oc(1 - exp);
  tmp.join_assign(1 + exp);
  known_result *= tmp;
  known_result -= tmp * Linear_Form<fl_r_oc>(B);
  tmp = fl_r_oc(-std::numeric_limits<float>::denorm_min());
  tmp.join_assign(std::numeric_limits<float>::denorm_min());
  known_result += tmp;

  nout << "*** known_result ***" << endl
       << known_result << endl;
  return result == known_result;
}

// Tests the linearization of A * B where A = [0, 1] and B = [2, 2].
bool
test07() {
  Box<fl_r_oc> store(2);
  fl_r_oc tmp(0);
  tmp.join_assign(1);
  store.set_interval(Variable(0), tmp);
  store.set_interval(Variable(1), fl_r_oc(2));
  var_fpess* var0 = new var_fpess(0);
  var_fpess* var1 = new var_fpess(1);
  mul_fpess mul(var0, var1);
  Float_Interval_Linear_Form result;
  mul.linearize(store, lsstr(), result);

  tmp = fl_r_oc(-std::numeric_limits<float>::denorm_min());
  tmp.join_assign(std::numeric_limits<float>::denorm_min());
  float exp = pow(2, -22);
  fl_r_oc coeff = fl_r_oc(2 - exp);
  coeff.join_assign(2 + exp);
  Float_Interval_Linear_Form known_result =
  Float_Interval_Linear_Form(Variable(0));
  known_result *= coeff;
  known_result += tmp;

  nout << "*** known_result ***" << endl
       << known_result << endl;
  return result == known_result;
}

// Tests the linearization of A / B where A = [0, 1] and B = [2, 2].
bool
test08() {
  Box<db_r_oc> store(2);
  db_r_oc tmp(0);
  tmp.join_assign(1);
  store.set_interval(Variable(0), tmp);
  store.set_interval(Variable(1), db_r_oc(2));
  var_fpedd* var0 = new var_fpedd(0);
  var_fpedd* var1 = new var_fpedd(1);
  div_fpedd div(var0, var1);
  Double_Interval_Linear_Form result;
  div.linearize(store, lddtr(), result);

  tmp = db_r_oc(-std::numeric_limits<double>::denorm_min());
  tmp.join_assign(std::numeric_limits<double>::denorm_min());
  float exp = pow(2, -53);
  db_r_oc coeff = db_r_oc(1 / 2.0 - exp);
  coeff.join_assign(1 / 2.0 + exp);
  Double_Interval_Linear_Form known_result =
  Double_Interval_Linear_Form(Variable(0));
  known_result *= coeff;
  known_result += tmp;

  nout << "*** known_result ***" << endl
       << known_result << endl;
  return result == known_result;
}

// Tests the linearization of [1/4, 1/2] * (-A) where A = [1, 10].
bool
test09() {
  Box<fl_r_oc> store(1);
  fl_r_oc tmp(1);
  tmp.join_assign(10);
  store.set_interval(Variable(0), tmp);
  con_fpesd* con = new con_fpesd(1 / 4.0, 1 / 2.0);
  var_fpesd* var0 = new var_fpesd(0);
  opp_fpesd* opp = new opp_fpesd(var0);
  mul_fpesd mul(con, opp);
  Float_Interval_Linear_Form result;
  mul.linearize(store, lsdtr(), result);

  Variable A(0);
  Float_Interval_Linear_Form known_result = Float_Interval_Linear_Form(A);
  float exp = pow(2, -53);
  tmp = fl_r_oc(-1 / 2.0 - exp);
  tmp.join_assign(-1 / 4.0 + exp);
  known_result *= tmp;
  tmp = fl_r_oc(-std::numeric_limits<float>::denorm_min());
  tmp.join_assign(std::numeric_limits<float>::denorm_min());
  known_result += tmp;

  nout << "*** known_result ***" << endl
       << known_result << endl;
  return result == known_result;
}

// Tests linearization of multiplication by unbounded operands.
bool
test10() {
  float max_fl = std::numeric_limits<float>::max();
  fl_r_oc min_fl = fl_r_oc(-std::numeric_limits<float>::denorm_min());
  min_fl.join_assign(std::numeric_limits<float>::denorm_min());
  Float_Interval_Linear_Form known_result1 =
  Float_Interval_Linear_Form(min_fl);
  con_fpess* con1 = new con_fpess(0, 0);
  con_fpess* con2 = new con_fpess(0, max_fl);
  sum_fpess* sum  = new sum_fpess(con1, con2);
  con_fpess* con3 = new con_fpess(0, 0);
  mul_fpess mul(con3, sum);
  Float_Interval_Linear_Form result;
  mul.linearize(Box<fl_r_oc>(), lsstr(), result);

  nout << "*** known_result1 ***" << endl
       << known_result1 << endl;
  bool ok1 = (known_result1 == result);

  double max_db = std::numeric_limits<double>::max();
  db_r_oc min_db = db_r_oc(-std::numeric_limits<double>::denorm_min());
  min_db.join_assign(std::numeric_limits<double>::denorm_min());
  Double_Interval_Linear_Form known_result2 =
  Double_Interval_Linear_Form(min_db);
  con_fpedd* con4 = new con_fpedd(0, 0);
  con_fpedd* con5 = new con_fpedd(0, max_db);
  sum_fpedd* sum2 = new sum_fpedd(con4, con5);
  con_fpedd* con6 = new con_fpedd(0, 0);
  mul_fpedd mul2(sum2, con6);
  Double_Interval_Linear_Form result2;
  mul2.linearize(Box<db_r_oc>(), lddtr(), result2);

  nout << "*** known_result2 ***" << endl
       << known_result2 << endl;
  bool ok2 = (known_result2 == result2);

  con_fpedd* con7 = new con_fpedd(0, 0);
  con_fpedd* con8 = new con_fpedd(0, max_db);
  sum_fpedd* sum3 = new sum_fpedd(con7, con8);
  con_fpedd* con9 = new con_fpedd(0, 0);
  con_fpedd* con10 = new con_fpedd(0, max_db);
  sum_fpedd* sum4 = new sum_fpedd(con9, con10);
  mul_fpedd mul3(sum3, sum4);

  bool ok3 = false;
  try {
    mul3.linearize(Box<db_r_oc>(), lddtr(), result2);
  }
  catch (Linearization_Failed e) {
    nout << "*** Linearization failed due to overflow. ***" << endl;
    ok3 = true;
  }

  return ok1 && ok2 && ok3;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
END_MAIN
