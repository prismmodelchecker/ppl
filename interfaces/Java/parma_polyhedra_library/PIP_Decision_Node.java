/* PIP_Decision_Node Java class declaration and implementation.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>

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


package parma_polyhedra_library;

import java.io.Writer;
import java.io.IOException;

//! An internal node of the PIP solution tree.
public class PIP_Decision_Node extends PIP_Tree_Node {

    /*! \brief
      Returns a the true branch (if \p b is not zero)
      or the false branch (if \p b is zero) of \p pip_dec.
    */
    /* FIXME: j_branch should be a boolean. */
    public native PIP_Tree_Node child_node(Integer j_branch);
}
