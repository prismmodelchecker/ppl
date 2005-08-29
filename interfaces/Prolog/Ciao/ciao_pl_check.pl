/* Prolog main program for the toy PPL/CIAO-Prolog predicate checker.
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

:- module(_, [main/0], []).
:- use_module(library(debugger)).
:- use_module(library(dynamic)).
:- use_module(library(lists)).
:- use_module(library(prolog_sys)).
:- use_module(library(read)).
:- use_module(library(write)).
:- use_module(ppl_ciao, [
	ppl_version_major/1,
	ppl_version_minor/1,
	ppl_version_revision/1,
	ppl_version_beta/1,
	ppl_version/1,
	ppl_banner/1,
	ppl_max_space_dimension/1,
	ppl_initialize/0,
	ppl_finalize/0,
	ppl_set_timeout_exception_atom/1,
	ppl_timeout_exception_atom/1,
	ppl_set_timeout/1,
	ppl_reset_timeout/0,
	ppl_new_Polyhedron_from_space_dimension/4,
	ppl_new_Polyhedron_from_Polyhedron/4,
	ppl_new_Polyhedron_from_constraints/3,
	ppl_new_Polyhedron_from_generators/3,
	ppl_new_Polyhedron_from_bounding_box/3,
	ppl_Polyhedron_swap/2,
	ppl_delete_Polyhedron/1,
	ppl_Polyhedron_space_dimension/2,
	ppl_Polyhedron_affine_dimension/2,
	ppl_Polyhedron_get_constraints/2,
	ppl_Polyhedron_get_minimized_constraints/2,
	ppl_Polyhedron_get_generators/2,
	ppl_Polyhedron_get_minimized_generators/2,
	ppl_Polyhedron_relation_with_constraint/3,
	ppl_Polyhedron_relation_with_generator/3,
        ppl_Polyhedron_get_bounding_box/3,
	ppl_Polyhedron_is_empty/1,
	ppl_Polyhedron_is_universe/1,
	ppl_Polyhedron_is_bounded/1,
	ppl_Polyhedron_bounds_from_above/2,
	ppl_Polyhedron_bounds_from_below/2,
	ppl_Polyhedron_maximize/5,
	ppl_Polyhedron_maximize_with_point/6,
	ppl_Polyhedron_minimize/5,
	ppl_Polyhedron_minimize_with_point/6,
        ppl_Polyhedron_is_topologically_closed/1,
	ppl_Polyhedron_contains_Polyhedron/2,
	ppl_Polyhedron_strictly_contains_Polyhedron/2,
	ppl_Polyhedron_is_disjoint_from_Polyhedron/2,
        ppl_Polyhedron_equals_Polyhedron/2,
	ppl_Polyhedron_OK/1,
	ppl_Polyhedron_add_constraint/2,
	ppl_Polyhedron_add_constraint_and_minimize/2,
	ppl_Polyhedron_add_generator/2,
	ppl_Polyhedron_add_generator_and_minimize/2,
	ppl_Polyhedron_add_constraints/2,
	ppl_Polyhedron_add_constraints_and_minimize/2,
	ppl_Polyhedron_add_generators/2,
	ppl_Polyhedron_add_generators_and_minimize/2,
	ppl_Polyhedron_intersection_assign/2,
	ppl_Polyhedron_intersection_assign_and_minimize/2,
	ppl_Polyhedron_poly_hull_assign/2,
	ppl_Polyhedron_poly_hull_assign_and_minimize/2,
	ppl_Polyhedron_poly_difference_assign/2,
	ppl_Polyhedron_affine_image/4,
	ppl_Polyhedron_affine_preimage/4,
	ppl_Polyhedron_bounded_affine_image/5,
	ppl_Polyhedron_bounded_affine_preimage/5,
	ppl_Polyhedron_generalized_affine_image/5,
	ppl_Polyhedron_generalized_affine_preimage/5,
	ppl_Polyhedron_generalized_affine_image_lhs_rhs/4,
	ppl_Polyhedron_generalized_affine_preimage_lhs_rhs/4,
	ppl_Polyhedron_time_elapse_assign/2,
        ppl_Polyhedron_topological_closure_assign/1,
	ppl_Polyhedron_BHRZ03_widening_assign_with_token/3,
	ppl_Polyhedron_BHRZ03_widening_assign/2,
	ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_token/4,
	ppl_Polyhedron_limited_BHRZ03_extrapolation_assign/3,
	ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_token/4,
	ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign/3,
	ppl_Polyhedron_H79_widening_assign_with_token/3,
	ppl_Polyhedron_H79_widening_assign/2,
	ppl_Polyhedron_limited_H79_extrapolation_assign_with_token/4,
	ppl_Polyhedron_limited_H79_extrapolation_assign/3,
	ppl_Polyhedron_bounded_H79_extrapolation_assign_with_token/4,
	ppl_Polyhedron_bounded_H79_extrapolation_assign/3,
	ppl_Polyhedron_add_space_dimensions_and_project/2,
	ppl_Polyhedron_add_space_dimensions_and_embed/2,
	ppl_Polyhedron_concatenate_assign/2,
	ppl_Polyhedron_remove_space_dimensions/2,
	ppl_Polyhedron_remove_higher_space_dimensions/2,
	ppl_Polyhedron_expand_space_dimension/3,
	ppl_Polyhedron_fold_space_dimensions/3,
	ppl_Polyhedron_map_space_dimensions/2
]).

:- set_prolog_flag(multi_arity_warnings, off).

:- include('pl_check.pl').

main:-
   (check_all ->
        write('OK')
   ;
        write('FAILURE')
   ),
   nl.
