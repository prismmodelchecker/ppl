/* Specialized "checked" functions for native floating-point numbers.
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
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#ifndef PPL_checked_float_inlines_hh
#define PPL_checked_float_inlines_hh 1

#include "Float.defs.hh"

namespace std {

inline double
fma(double x, double y, double z) {
#if HAVE_DECL_FMA
  return ::fma(x, y, z);
#else
  return x*y + z;
#endif
}

inline float
fma(float x, float y, float z) {
#if HAVE_DECL_FMAF
  return ::fmaf(x, y, z);
#else
  return x*y + z;
#endif
}

inline long double
fma(long double x, long double y, long double z) {
#if HAVE_DECL_FMAL
  return ::fmal(x, y, z);
#else
  return x*y + z;
#endif
}

}

namespace Parma_Polyhedra_Library {

namespace Checked {

template <typename Policy, typename T>
inline Result
classify_float(const T v, bool nan, bool inf, bool sign) {
  Float<T> f(v);
  if ((nan || sign) && f.is_nan())
    return VC_NAN;
  if (inf) {
    int i = f.is_inf();
    if (i < 0)
      return VC_MINUS_INFINITY;
    if (i > 0)
      return VC_PLUS_INFINITY;
  }
  if (sign) {
    if (v < 0)
      return V_LT;
    if (v > 0)
      return V_GT;
    return V_EQ;
  }
  return VC_NORMAL;
}

template <typename Policy, typename T>
inline bool
is_nan_float(const T v) {
  Float<T> f(v);
  return f.is_nan();
}

template <typename Policy, typename T>
inline bool
is_minf_float(const T v) {
  Float<T> f(v);
  return f.is_inf() < 0;
}

template <typename Policy, typename T>
inline bool
is_pinf_float(const T v) {
  Float<T> f(v);
  return f.is_inf() > 0;
}

template <typename Policy, typename T>
inline Result
set_special_float(T& v, Result r) {
  switch (classify(r)) {
  case VC_MINUS_INFINITY:
    v = -HUGE_VAL;
    break;
  case VC_PLUS_INFINITY:
    v = HUGE_VAL;
    break;
  case VC_NAN:
    v = NAN;
    break;
  default:
    break;
  }
  return r;
}

template <typename T>
inline void
pred_float(T& v) {
  Float<T> f(v);
  assert(!f.is_nan());
  assert(f.is_inf() >= 0);
  if (f.is_zero() > 0) {
    f.negate();
    f.inc();
  }
  else if (f.sign_bit()) {
    f.inc();
  }
  else {
    f.dec();
  }
  v = f.value();
}

template <typename T>
inline void
succ_float(T& v) {
  Float<T> f(v);
  assert(!f.is_nan());
  assert(f.is_inf() <= 0);
  if (f.is_zero() < 0) {
    f.negate();
    f.inc();
  }
  else if (!f.sign_bit()) {
    f.inc();
  }
  else {
    f.dec();
  }
  v = f.value();
}

SPECIALIZE_CLASSIFY(float, float32_t)
SPECIALIZE_IS_NAN(float, float32_t)
SPECIALIZE_IS_MINF(float, float32_t)
SPECIALIZE_IS_PINF(float, float32_t)
SPECIALIZE_SET_SPECIAL(float, float32_t)

SPECIALIZE_CLASSIFY(float, float64_t)
SPECIALIZE_IS_NAN(float, float64_t)
SPECIALIZE_IS_MINF(float, float64_t)
SPECIALIZE_IS_PINF(float, float64_t)
SPECIALIZE_SET_SPECIAL(float, float64_t)

#ifdef FLOAT96_TYPE
SPECIALIZE_CLASSIFY(float, float96_t)
SPECIALIZE_IS_NAN(float, float96_t)
SPECIALIZE_IS_MINF(float, float96_t)
SPECIALIZE_IS_PINF(float, float96_t)
SPECIALIZE_SET_SPECIAL(float, float96_t)
#endif

#ifdef FLOAT128_TYPE
SPECIALIZE_CLASSIFY(float, float128_t)
SPECIALIZE_IS_NAN(float, float128_t)
SPECIALIZE_IS_MINF(float, float128_t)
SPECIALIZE_IS_PINF(float, float128_t)
SPECIALIZE_SET_SPECIAL(float, float128_t)
#endif

template <typename Policy, typename To>
inline Result
round_lt_float(To& to, Rounding_Dir dir) {
  if (rounding_direction(dir) == ROUND_DOWN) {
    pred_float(to);
    return V_GT;
  }
  return V_LT;
}

template <typename Policy, typename To>
inline Result
round_gt_float(To& to, Rounding_Dir dir) {
  if (rounding_direction(dir) == ROUND_UP) {
    succ_float(to);
    return V_LT;
  }
  return V_GT;
}

template <typename Policy>
inline void
prepare_inexact() {
  if (Policy::fpu_check_inexact)
    fpu_reset_inexact();
}

template <typename Policy>
inline Result 
result_relation(Rounding_Dir dir) {
  if (Policy::fpu_check_inexact && !fpu_check_inexact())
    return V_EQ;
  else if (want_rounding<Policy>(dir)) {
    switch (rounding_direction(dir)) {
    case ROUND_DOWN:
      return Policy::fpu_check_inexact ? V_GT : V_GE;
    case ROUND_UP:
      return Policy::fpu_check_inexact ? V_LT : V_LE;
    default:
      return Policy::fpu_check_inexact ? V_NE : V_LGE;
    }
  } else
    return Policy::fpu_check_inexact ? V_NE : V_LGE;
}

template <typename Policy, typename From, typename To>
inline Result 
assign_float_float_exact(To& to, const From from, Rounding_Dir) {
  to = from;
  return V_EQ;
}

template <typename Policy, typename To, typename From>
inline Result
assign_float_float(To& to, const From from, Rounding_Dir dir) {
  prepare_inexact<Policy>();
  to = from;
  return result_relation<Policy>(dir);
}

template <typename Policy, typename Type>
inline Result 
assign_result_inexact(Type& to, const Type from, Rounding_Dir dir) {
  to = from;
  return result_relation<Policy>(dir);
}

template <typename Policy, typename Type>
inline Result 
neg_float(Type& to, const Type from, Rounding_Dir dir) {
  return assign_float_float_exact<Policy>(to, -from, dir);
}

template <typename Policy, typename Type>
inline Result 
add_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, x + y, dir);
}

template <typename Policy, typename Type>
inline Result 
sub_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, x - y, dir);
}

template <typename Policy, typename Type>
inline Result 
mul_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, x * y, dir);
}

template <typename Policy, typename Type>
inline Result 
div_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (Policy::check_divbyzero && y == 0) {
    to = NAN;
    return V_DIV_ZERO;
  }
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, x / y, dir);
}

template <typename Policy, typename Type>
inline Result 
rem_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (Policy::check_divbyzero && y == 0) {
    to = NAN;
    return V_MOD_ZERO;
  }
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, std::fmod(x, y, dir));
}

template <typename Policy, typename Type>
inline Result
abs_float(Type& to, const Type from, Rounding_Dir dir) {
  return assign_float_float_exact<Policy>(to, std::abs(from), dir);
}

template <typename Policy, typename Type>
inline Result
sqrt_float(Type& to, const Type from, Rounding_Dir dir) {
  if (Policy::check_sqrt_neg && from < 0) {
    to = NAN;
    return V_SQRT_NEG;
  }
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, std::sqrt(from), dir);
}

template <typename Policy, typename Type>
inline Result
sgn_float(const Type x) {
  return classify<Policy>(x, false, false, true);
}

template <typename Policy, typename Type>
inline Result
cmp_float(const Type x, const Type y) {
  if (x > y)
    return V_GT;
  if (x < y)
    return V_LT;
  if (x == y)
    return V_EQ;
  return V_UNORD_COMP;
}

template <typename Policy, typename To, typename From>
inline Result 
assign_float_int_exact(To& to, const From from, Rounding_Dir) {
  to = from;
  return V_EQ;
}

template <typename Policy, typename To, typename From>
inline Result
assign_float_int(To& to, const From from, Rounding_Dir dir) {
  prepare_inexact<Policy>();
  to = from;
  return result_relation<Policy>(dir);
}

template <typename Policy, typename Type>
inline Result
set_neg_overflow_float(Type& to, Rounding_Dir dir) {
  switch (rounding_direction(dir)) {
  case ROUND_UP:
    {
      Float<Type> f(-HUGE_VAL);
      f.dec();
      to = f.value();
      return V_LT;
    }
  default:
    to = -HUGE_VAL;
    return V_GT;
  }
}

template <typename Policy, typename Type>
inline Result
set_pos_overflow_float(Type& to, Rounding_Dir dir) {
  switch (rounding_direction(dir)) {
  case ROUND_DOWN:
    {
      Float<Type> f(HUGE_VAL);
      f.dec();
      to = f.value();
      return V_GT;
    }
  default:
    to = HUGE_VAL;
    return V_LT;
  }
}

template <typename Policy, typename Type>
inline Result
assign_float_mpz(Type& to, const mpz_class& _from, Rounding_Dir dir)
{
  mpz_srcptr from = _from.get_mpz_t();
  int sign = mpz_sgn(from);
  if (sign == 0) {
    to = 0;
    return V_EQ;
  }
  size_t exponent = mpz_sizeinbase(from, 2) - 1;
  if (exponent >= 1 << (Float<Type>::EXPONENT_BITS - 1)) {
    if (sign < 0)
      return set_neg_overflow_float<Policy>(to, dir);
    else
      return set_pos_overflow_float<Policy>(to, dir);
  }
  unsigned long zeroes = mpn_scan1(from->_mp_d, 0);
  size_t significative_bits = exponent - zeroes;
  mpz_t mantissa;
  mpz_init(mantissa);
  if (exponent > Float<Type>::MANTISSA_BITS)
    mpz_tdiv_q_2exp(mantissa, from, exponent - Float<Type>::MANTISSA_BITS);
  else
    mpz_mul_2exp(mantissa, from, Float<Type>::MANTISSA_BITS - exponent);
  Float<Type> f(to);
  f.build(sign < 0, mantissa, exponent);
  to = f.value();
  if (significative_bits > Float<Type>::MANTISSA_BITS) {
    if (sign < 0)
      return round_lt_float<Policy>(to, dir);
    else
      return round_gt_float<Policy>(to, dir);
  }
  return V_EQ;
}

template <typename Policy, typename Type>
inline Result 
add_mul_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, std::fma(to, x, y), dir);
}

template <typename Policy, typename Type>
inline Result 
sub_mul_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  prepare_inexact<Policy>();
  return assign_result_inexact<Policy>(to, std::fma(to, x, -y), dir);
}

template <typename T>
T strtod_(const char* nptr, char** endptr);

#if !HAVE_DECL_STRTOF
float strtof(const char* nptr, char** endptr);
#endif

template <>
inline float
strtod_(const char* nptr, char** endptr) {
  return strtof(nptr, endptr);
}

#if !HAVE_DECL_STRTOD
double strtod(const char* nptr, char** endptr);
#endif

template <>
inline double
strtod_(const char* nptr, char** endptr) {
  return strtod(nptr, endptr);
}

#if !HAVE_DECL_STRTOLD
long double
strtold(const char* nptr, char** endptr);
#endif

template <>
inline long double
strtod_(const char*nptr, char** endptr) {
  return strtold(nptr, endptr);
}

template <typename T>
inline int
dtostr_(char* str, size_t size, T x) {
  return snprintf(str, size, "%.99g", static_cast<double>(x));
}

template <>
inline int
dtostr_<long double>(char* str, size_t size, long double x) {
  return snprintf(str, size, "%.99Lg", x);
}

template <typename Policy, typename Type>
inline Result
from_c_string_float(Type& to, const char* from, Rounding_Dir dir) {
  errno = 0;
  char* end;
  Type v = strtod_<Type>(from, &end);
  if (errno == ERANGE) {
    to = v;
    if (v < 0)
      return V_GT;
    if (v > 0)
      return V_LT;
    // FIXME:
    return V_EQ;
  }
  if (errno || *end)
    return set_special<Policy>(to, V_CVT_STR_UNK);
  to = v;
  // FIXME:
  return assign_float_float_exact<Policy>(to, v, dir);
}

template <typename Policy, typename Type>
inline Result
to_c_string_float(char* str, size_t size, Type& from, const Numeric_Format&, Rounding_Dir) {
  if (from == 0) {
    strncpy(str, "0", size);
    return V_EQ;
  }
  dtostr_(str, size, from);
  return V_EQ;
}

template <typename Policy, typename To>
inline Result
assign_float_minf(To& to, const Minus_Infinity&, Rounding_Dir) {
  to = -HUGE_VAL;
  return V_EQ;
}

template <typename Policy, typename To>
inline Result
assign_float_pinf(To& to, const Plus_Infinity&, Rounding_Dir) {
  to = HUGE_VAL;
  return V_EQ;
}

template <typename Policy, typename To>
inline Result
assign_float_nan(To& to, const Not_A_Number&, Rounding_Dir) {
  to = NAN;
  return V_EQ;
}

#define ASSIGN_R2(Smaller, Larger) \
SPECIALIZE_ASSIGN(float_float_exact, Larger, Smaller) \
SPECIALIZE_ASSIGN(float_float, Smaller, Larger)

SPECIALIZE_ASSIGN(float_int_exact, float32_t, int8_t)
SPECIALIZE_ASSIGN(float_int_exact, float32_t, int16_t)
SPECIALIZE_ASSIGN(float_int_exact, float32_t, uint8_t)
SPECIALIZE_ASSIGN(float_int_exact, float32_t, uint16_t)
SPECIALIZE_ASSIGN(float_int, float32_t, int32_t)
SPECIALIZE_ASSIGN(float_int, float32_t, int64_t)
SPECIALIZE_ASSIGN(float_int, float32_t, uint32_t)
SPECIALIZE_ASSIGN(float_int, float32_t, uint64_t)
SPECIALIZE_ASSIGN(float_float, float32_t, float32_t)
ASSIGN_R2(float32_t, float64_t)

SPECIALIZE_ASSIGN(float_int_exact, float64_t, int8_t)
SPECIALIZE_ASSIGN(float_int_exact, float64_t, int16_t)
SPECIALIZE_ASSIGN(float_int_exact, float64_t, int32_t)
SPECIALIZE_ASSIGN(float_int_exact, float64_t, uint8_t)
SPECIALIZE_ASSIGN(float_int_exact, float64_t, uint16_t)
SPECIALIZE_ASSIGN(float_int_exact, float64_t, uint32_t)
SPECIALIZE_ASSIGN(float_int, float64_t, int64_t)
SPECIALIZE_ASSIGN(float_int, float64_t, uint64_t)
SPECIALIZE_ASSIGN(float_float, float64_t, float64_t)

#ifdef FLOAT96_TYPE
SPECIALIZE_ASSIGN(float_int_exact, float96_t, int8_t)
SPECIALIZE_ASSIGN(float_int_exact, float96_t, int16_t)
SPECIALIZE_ASSIGN(float_int_exact, float96_t, int32_t)
SPECIALIZE_ASSIGN(float_int_exact, float96_t, int64_t)
SPECIALIZE_ASSIGN(float_int_exact, float96_t, uint8_t)
SPECIALIZE_ASSIGN(float_int_exact, float96_t, uint16_t)
SPECIALIZE_ASSIGN(float_int_exact, float96_t, uint32_t)
SPECIALIZE_ASSIGN(float_int_exact, float96_t, uint64_t)
SPECIALIZE_ASSIGN(float_float, float96_t, float96_t)
ASSIGN_R2(float32_t, float96_t)
ASSIGN_R2(float64_t, float96_t)
#endif

#ifdef FLOAT128_TYPE
SPECIALIZE_ASSIGN(float_int_exact, float128_t, int8_t)
SPECIALIZE_ASSIGN(float_int_exact, float128_t, int16_t)
SPECIALIZE_ASSIGN(float_int_exact, float128_t, int32_t)
SPECIALIZE_ASSIGN(float_int_exact, float128_t, int64_t)
SPECIALIZE_ASSIGN(float_int_exact, float128_t, uint8_t)
SPECIALIZE_ASSIGN(float_int_exact, float128_t, uint16_t)
SPECIALIZE_ASSIGN(float_int_exact, float127_t, uint32_t)
SPECIALIZE_ASSIGN(float_int_exact, float128_t, uint64_t)
SPECIALIZE_ASSIGN(float_float, float128_t, float128_t)
ASSIGN_R2(float32_t, float128_t)
ASSIGN_R2(float64_t, float128_t)
#ifdef FLOAT96_TYPE
ASSIGN_R2(float96_t, float128_t)
#endif
#endif

#undef ASSIGN_R2

SPECIALIZE_ASSIGN(float_minf, float, Minus_Infinity)
SPECIALIZE_ASSIGN(float_pinf, float, Plus_Infinity)
SPECIALIZE_ASSIGN(float_nan, float, Not_A_Number)
SPECIALIZE_NEG(float, float, float)
SPECIALIZE_ABS(float, float, float)
SPECIALIZE_ADD(float, float, float)
SPECIALIZE_SUB(float, float, float)
SPECIALIZE_MUL(float, float, float)
SPECIALIZE_DIV(float, float, float)
SPECIALIZE_REM(float, float, float)
SPECIALIZE_SQRT(float, float, float)
SPECIALIZE_GCD(generic, float, float)
SPECIALIZE_LCM(generic, float, float)
SPECIALIZE_SGN(float, float)
SPECIALIZE_CMP(float, float, float)
SPECIALIZE_ADD_MUL(float, float, float)
SPECIALIZE_SUB_MUL(float, float, float)
SPECIALIZE_FROM_C_STRING(float, float)
SPECIALIZE_TO_C_STRING(float, float)

SPECIALIZE_ASSIGN(float_minf, double, Minus_Infinity)
SPECIALIZE_ASSIGN(float_pinf, double, Plus_Infinity)
SPECIALIZE_ASSIGN(float_nan, double, Not_A_Number)
SPECIALIZE_NEG(float, double, double)
SPECIALIZE_ABS(float, double, double)
SPECIALIZE_ADD(float, double, double)
SPECIALIZE_SUB(float, double, double)
SPECIALIZE_MUL(float, double, double)
SPECIALIZE_DIV(float, double, double)
SPECIALIZE_REM(float, double, double)
SPECIALIZE_SQRT(float, double, double)
SPECIALIZE_GCD(generic, double, double)
SPECIALIZE_LCM(generic, double, double)
SPECIALIZE_SGN(float, double)
SPECIALIZE_CMP(float, double, double)
SPECIALIZE_ADD_MUL(float, double, double)
SPECIALIZE_SUB_MUL(float, double, double)
SPECIALIZE_FROM_C_STRING(float, double)
SPECIALIZE_TO_C_STRING(float, double)

SPECIALIZE_ASSIGN(float_minf, long double, Minus_Infinity)
SPECIALIZE_ASSIGN(float_pinf, long double, Plus_Infinity)
SPECIALIZE_ASSIGN(float_nan, long double, Not_A_Number)
SPECIALIZE_NEG(float, long double, long double)
SPECIALIZE_ABS(float, long double, long double)
SPECIALIZE_ADD(float, long double, long double)
SPECIALIZE_SUB(float, long double, long double)
SPECIALIZE_MUL(float, long double, long double)
SPECIALIZE_DIV(float, long double, long double)
SPECIALIZE_REM(float, long double, long double)
SPECIALIZE_SQRT(float, long double, long double)
SPECIALIZE_GCD(generic, long double, long double)
SPECIALIZE_LCM(generic, long double, long double)
SPECIALIZE_SGN(float, long double)
SPECIALIZE_CMP(float, long double, long double)
SPECIALIZE_ADD_MUL(float, long double, long double)
SPECIALIZE_SUB_MUL(float, long double, long double)
SPECIALIZE_FROM_C_STRING(float, long double)
SPECIALIZE_TO_C_STRING(float, long double)

SPECIALIZE_ASSIGN(float_mpz, float, mpz_class)
SPECIALIZE_ASSIGN(float_mpz, double, mpz_class)
SPECIALIZE_ASSIGN(float_mpz, long double, mpz_class)

} // namespace Checked

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_checked_int_inlines_hh)
