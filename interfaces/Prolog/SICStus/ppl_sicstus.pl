foreign_resource(ppl_sicstus,
[
	init(ppl_init),
	deinit(ppl_deinit),
	ppl_new_polyhedron,
	ppl_new_empty_polyhedron,
	ppl_delete_polyhedron,
	ppl_space_dimension,
	ppl_insert_constraint,
	ppl_insert_generator,
	ppl_remove_dimensions,
	ppl_check_empty,
	ppl_get_constraints,
	ppl_get_generators,
	ppl_intersection_assign,
	ppl_convex_hull_assign,
	ppl_widening_assign
]).

foreign(ppl_new_polyhedron,       c,
	ppl_new_polyhedron([-address], +integer)).
foreign(ppl_new_empty_polyhedron, c,
	ppl_new_empty_polyhedron([-address], +integer)).
foreign(ppl_delete_polyhedron,    c, ppl_delete_polyhedron(+address)).
foreign(ppl_space_dimension,      c,
	ppl_space_dimension(+address, [-integer])).
foreign(ppl_insert_constraint,    c, ppl_insert_constraint(+address, +term)).
foreign(ppl_insert_generator,     c, ppl_insert_generator(+address, +term)).
foreign(ppl_remove_dimensions,    c, ppl_remove_dimensions(+address, +term)).
foreign(ppl_check_empty,          c, ppl_check_empty(+address, [-integer])).
foreign(ppl_get_constraints,      c, ppl_get_constraints(+address, -term)).
foreign(ppl_get_generators,       c, ppl_get_generators(+address, -term)).
foreign(ppl_intersection_assign,  c,
	ppl_intersection_assign(+address, +address)).
foreign(ppl_convex_hull_assign,   c,
	ppl_convex_hull_assign(+address, +address)).
foreign(ppl_widening_assign,      c, ppl_widening_assign(+address, +address)).

:- load_foreign_resource(ppl_sicstus).

ppl_check_empty(Polyhedron) :-
  ppl_check_empty(Polyhedron, 1).
