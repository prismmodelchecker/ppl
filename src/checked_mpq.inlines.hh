/* Specialized "checked" functions for GMP's mpq_class numbers.
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

#ifndef PPL_checked_mpq_inlines_hh
#define PPL_checked_mpq_inlines_hh 1

#include <sstream>
#include <climits>
#include <stdexcept>

namespace Parma_Polyhedra_Library {

namespace Checked {

template <typename Policy>
inline Result
classify_mpq(const mpq_class& v, bool nan, bool inf, bool sign) {
  if ((Policy::has_nan || Policy::has_infinity)
      && ::sgn(v.get_den()) == 0) {
    int s = ::sgn(v.get_num());
    if (Policy::has_nan && (nan || sign) && s == 0)
      return VC_NAN;
    if (!inf && !sign)
      return VC_NORMAL;
    if (Policy::has_infinity) {
      if (s < 0)
	return inf ? VC_MINUS_INFINITY : V_LT;
      if (s > 0)
	return inf ? VC_PLUS_INFINITY : V_GT;
    }
  }
  if (sign)
    return sgn<Policy>(v);
  return VC_NORMAL;
}

PPL_SPECIALIZE_CLASSIFY(classify_mpq, mpq_class)

template <typename Policy>
inline bool
is_nan_mpq(const mpq_class& v) {
  return Policy::has_nan
    && ::sgn(v.get_den()) == 0
    && ::sgn(v.get_num()) == 0;
}

PPL_SPECIALIZE_IS_NAN(is_nan_mpq, mpq_class)

template <typename Policy>
inline bool
is_minf_mpq(const mpq_class& v) {
  return Policy::has_infinity
    && ::sgn(v.get_den()) == 0
    && ::sgn(v.get_num()) < 0;
}

PPL_SPECIALIZE_IS_MINF(is_minf_mpq, mpq_class)

template <typename Policy>
inline bool
is_pinf_mpq(const mpq_class& v) {
  return Policy::has_infinity
    && ::sgn(v.get_den()) == 0
    && ::sgn(v.get_num()) > 0;
}

PPL_SPECIALIZE_IS_PINF(is_pinf_mpq, mpq_class)

template <typename Policy>
inline bool
is_int_mpq(const mpq_class& v) {
  if ((Policy::has_infinity || Policy::has_nan)
      && ::sgn(v.get_den()) == 0)
    return !(Policy::has_nan && ::sgn(v.get_num()) == 0);
  else
    return v.get_den() == 1;
}

PPL_SPECIALIZE_IS_INT(is_int_mpq, mpq_class)

template <typename Policy>
inline Result
assign_special_mpq(mpq_class& v, Result r, Rounding_Dir) {
  Result c = classify(r);
  if (Policy::has_nan && c == VC_NAN) {
    v.get_num() = 0;
    v.get_den() = 0;
  }
  else if (Policy::has_infinity) {
    switch (c) {
    case VC_MINUS_INFINITY:
      v.get_num() = -1;
      v.get_den() = 0;
      return V_EQ;
    case VC_PLUS_INFINITY:
      v.get_num() = 1;
      v.get_den() = 0;
      return V_EQ;
    default:
      break;
    }
  }
  return r;
}

PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_mpq, mpq_class)

PPL_SPECIALIZE_COPY(copy_generic, mpq_class)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
construct_mpq_base(mpq_class& to, const From& from, Rounding_Dir) {
  new (&to) mpq_class(from);
  return V_EQ;
}

PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, mpz_class)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed short)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed int)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed long)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned short)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned int)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
construct_mpq_float(mpq_class& to, const From& from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  new (&to) mpq_class(from);
  return V_EQ;
}

PPL_SPECIALIZE_CONSTRUCT(construct_mpq_float, mpq_class, float)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_float, mpq_class, double)

PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed short)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed int)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed long)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned short)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned int)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpq_float(mpq_class& to, const From& from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  to = from;
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpq_float, mpq_class, float)
PPL_SPECIALIZE_ASSIGN(assign_mpq_float, mpq_class, double)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpq_signed_int(mpq_class& to, const From from, Rounding_Dir) {
  if (sizeof(From) <= sizeof(signed long))
    to = static_cast<signed long>(from);
  else {
    mpz_ptr m = to.get_num().get_mpz_t();
    if (from >= 0)
      mpz_import(m, 1, 1, sizeof(From), 0, 0, &from);
    else {
      From n = -from;
      mpz_import(m, 1, 1, sizeof(From), 0, 0, &n);
      mpz_neg(m, m);
    }
    to.get_den() = 1;
  }
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpq_signed_int, mpq_class, signed long long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpq_unsigned_int(mpq_class& to, const From from, Rounding_Dir) {
  if (sizeof(From) <= sizeof(unsigned long))
    to = static_cast<unsigned long>(from);
  else {
    mpz_import(to.get_num().get_mpz_t(), 1, 1, sizeof(From), 0, 0, &from);
    to.get_den() = 1;
  }
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpq_unsigned_int, mpq_class, unsigned long long)

template <typename To_Policy, typename From_Policy>
inline Result
floor_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpz_fdiv_q(to.get_num().get_mpz_t(),
	     from.get_num().get_mpz_t(), from.get_den().get_mpz_t());
  to.get_den() = 1;
  return V_EQ;
}

PPL_SPECIALIZE_FLOOR(floor_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
ceil_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpz_cdiv_q(to.get_num().get_mpz_t(),
	     from.get_num().get_mpz_t(), from.get_den().get_mpz_t());
  to.get_den() = 1;
  return V_EQ;
}

PPL_SPECIALIZE_CEIL(ceil_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
trunc_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpz_tdiv_q(to.get_num().get_mpz_t(),
	     from.get_num().get_mpz_t(), from.get_den().get_mpz_t());
  to.get_den() = 1;
  return V_EQ;
}

PPL_SPECIALIZE_TRUNC(trunc_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
neg_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpq_neg(to.get_mpq_t(), from.get_mpq_t());
  return V_EQ;
}

PPL_SPECIALIZE_NEG(neg_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
add_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  to = x + y;
  return V_EQ;
}

PPL_SPECIALIZE_ADD(add_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
sub_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  to = x - y;
  return V_EQ;
}

PPL_SPECIALIZE_SUB(sub_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
mul_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  to = x * y;
  return V_EQ;
}

PPL_SPECIALIZE_MUL(mul_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
div_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, sgn(y) == 0))
    return assign_special<To_Policy>(to, V_DIV_ZERO, ROUND_IGNORE);
  to = x / y;
  return V_EQ;
}

PPL_SPECIALIZE_DIV(div_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
idiv_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_div_zero, sgn(y) == 0))
    return assign_special<To_Policy>(to, V_DIV_ZERO, ROUND_IGNORE);
  to = x / y;
  return trunc<To_Policy, To_Policy>(to, to, dir);
}

PPL_SPECIALIZE_IDIV(idiv_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
rem_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, sgn(y) == 0))
    return assign_special<To_Policy>(to, V_MOD_ZERO, ROUND_IGNORE);
  to = x / y;
  to.get_num() %= to.get_den();
  return V_EQ;
}

PPL_SPECIALIZE_REM(rem_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
mul2exp_mpq(mpq_class& to, const mpq_class& x, int exp, Rounding_Dir dir) {
  if (exp < 0)
    return div2exp<To_Policy, From_Policy>(to, x, -exp, dir);
  mpz_mul_2exp(to.get_num().get_mpz_t(), x.get_num().get_mpz_t(), exp);
  to.get_den() = x.get_den();
  to.canonicalize();
  return V_EQ;
}

PPL_SPECIALIZE_MUL2EXP(mul2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
div2exp_mpq(mpq_class& to, const mpq_class& x, int exp, Rounding_Dir dir) {
  if (exp < 0)
    return mul2exp<To_Policy, From_Policy>(to, x, -exp, dir);
  to.get_num() = x.get_num();
  mpz_mul_2exp(to.get_den().get_mpz_t(), x.get_den().get_mpz_t(), exp);
  to.canonicalize();
  return V_EQ;
}

PPL_SPECIALIZE_DIV2EXP(div2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
abs_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  to = abs(from);
  return V_EQ;
}

PPL_SPECIALIZE_ABS(abs_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
add_mul_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y,
	    Rounding_Dir) {
  to += x * y;
  return V_EQ;
}

PPL_SPECIALIZE_ADD_MUL(add_mul_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
sub_mul_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y,
	    Rounding_Dir) {
  to -= x * y;
  return V_EQ;
}

PPL_SPECIALIZE_SUB_MUL(sub_mul_mpq, mpq_class, mpq_class, mpq_class)

extern unsigned long rational_sqrt_precision_parameter;

template <typename To_Policy, typename From_Policy>
inline Result
sqrt_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_sqrt_neg, from < 0))
    return assign_special<To_Policy>(to, V_SQRT_NEG, ROUND_IGNORE);
  const unsigned long k = rational_sqrt_precision_parameter;
  mpz_class& to_num = to.get_num();
  mul2exp<To_Policy, From_Policy>(to_num, from.get_num(), 2*k, dir);
  Result rdiv
    = div<To_Policy, To_Policy, To_Policy>(to_num,
					   to_num, from.get_den(), dir);
  Result rsqrt = sqrt<To_Policy, To_Policy>(to_num, to_num, dir);
  mpz_class& to_den = to.get_den();
  to_den = 1;
  mul2exp<To_Policy, To_Policy>(to_den, to_den, k, dir);
  to.canonicalize();
  return rdiv != V_EQ ? rdiv : rsqrt;
}

PPL_SPECIALIZE_SQRT(sqrt_mpq, mpq_class, mpq_class)

template <typename Policy>
inline Result
input_mpq(mpq_class& to, std::istream& is, Rounding_Dir) {
  Result r = input_mpq(to, is);
  switch (classify(r)) {
  case VC_MINUS_INFINITY:
  case VC_PLUS_INFINITY:
  case VC_NAN:
    return assign_special<Policy>(to, r, ROUND_IGNORE);
  default:
    return r;
  }
}

PPL_SPECIALIZE_INPUT(input_mpq, mpq_class)

template <typename Policy>
inline Result
output_mpq(std::ostream& os,
	   const mpq_class& from,
	   const Numeric_Format&,
	   Rounding_Dir) {
  os << from;
  return V_EQ;
}

PPL_SPECIALIZE_OUTPUT(output_mpq, mpq_class)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpq_long_double(mpq_class& to, const From& from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  // FIXME: this is an incredibly inefficient implementation!
  std::stringstream ss;
  output<From_Policy>(ss, from, Numeric_Format(), dir);
  return input_mpq(to, ss);
}

PPL_SPECIALIZE_ASSIGN(assign_mpq_long_double, mpq_class, long double)

} // namespace Checked

//! Returns the precision parameter used for rational square root calculations.
inline unsigned
rational_sqrt_precision_parameter() {
  return Checked::rational_sqrt_precision_parameter;
}

//! Sets the precision parameter used for rational square root calculations.
/*!
  If \p p is less than or equal to <CODE>INT_MAX</CODE>, sets the
  precision parameter used for rational square root calculations to \p p.

  \exception std::invalid_argument
  Thrown if \p p is greater than <CODE>INT_MAX</CODE>.
*/
inline void
set_rational_sqrt_precision_parameter(const unsigned p) {
  if (p <= INT_MAX)
    Checked::rational_sqrt_precision_parameter = p;
  else
    throw std::invalid_argument("PPL::set_rational_sqrt_precision_parameter(p)"
				" with p > INT_MAX");
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_checked_mpq_inlines_hh)
