m4_divert(-1)

dnl This m4 file defines the list of methods
dnl for the Java interface; this includes:
dnl - the list in the imported file and any Java specific methods.

dnl Copyright (C) 2001-2008 Roberto Bagnara <bagnara@cs.unipr.it>
dnl
dnl This file is part of the Parma Polyhedra Library (PPL).
dnl
dnl The PPL is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the
dnl Free Software Foundation; either version 3 of the License, or (at your
dnl option) any later version.
dnl
dnl The PPL is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
dnl FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software Foundation,
dnl Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
dnl
dnl For the most up-to-date information see the Parma Polyhedra Library
dnl site: http://www.cs.unipr.it/ppl/ .

dnl Include the language independent macros.
m4_include(`ppl_interface_generator_common_procedure_generators.m4')
m4_include(`ppl_interface_generator_common.m4')
m4_include(`ppl_interface_generator_common_dat.m4')

dnl m4_procedure_list
dnl This class extends the m4_common_procedure_list
dnl and all procedures common to the all the interfaces should go there.
dnl
dnl Note that the code for the schema "<name>_code" must be defined
dnl in the ppl_interface_generator_*_code.m4 file.
dnl The <name> must be exactly as written here.

m4_define(`m4_procedure_list', `m4_echo_unquoted(`m4_common_procedure_list',
ppl_free_@CLASS@/1 +all,
ppl_@CLASS@_hashcode/2 +all -box,
ppl_@CLASS@_string/1 +all)dnl
')

dnl TODO
dnl Some methods are not yet implemented for the Java interface:
dnl ppl_@CLASS@_ascii_dump/1
dnl ppl_@CLASS@_@PARTITION@/4 +pointset_powerset \grid,
dnl ppl_@CLASS@_approximate_partition/5 +pointset_powerset \shape,
dnl ppl_@CLASS@_BHZ03_@ALT_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign/2
dnl ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign/3
dnl ppl_@CLASS@_BGP99_@DISJUNCT_EXTRAPOLATION@_extrapolation_assign/3
