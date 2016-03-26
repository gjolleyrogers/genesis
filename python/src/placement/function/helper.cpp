/**
 * @brief
 *
 * @file
 * @ingroup python
 */

#include <python/src/common.hpp>

#include "lib/genesis.hpp"

using namespace ::genesis::placement;

PYTHON_EXPORT_FUNCTIONS(placement_function_helper_export, "placement")
{

    boost::python::def(
        "has_correct_edge_nums",
        ( bool ( * )( PlacementTree const & ))( &::genesis::placement::has_correct_edge_nums ),
        ( boost::python::arg("tree") ),
        get_docstring("bool ::genesis::placement::has_correct_edge_nums (PlacementTree const & tree)")
    );

    boost::python::def(
        "validate",
        ( bool ( * )( Sample const &, bool, bool ))( &::genesis::placement::validate ),
        ( boost::python::arg("smp"), boost::python::arg("check_values"), boost::python::arg("break_on_values") ),
        get_docstring("bool ::genesis::placement::validate (Sample const & smp, bool check_values, bool break_on_values)")
    );

    boost::python::def(
        "edge_num_to_edge_map",
        ( std::unordered_map< int, PlacementTree::EdgeType * > ( * )( PlacementTree const & ))( &::genesis::placement::edge_num_to_edge_map ),
        ( boost::python::arg("tree") ),
        get_docstring("std::unordered_map< int, PlacementTree::EdgeType * > ::genesis::placement::edge_num_to_edge_map (PlacementTree const & tree)")
    );

    boost::python::def(
        "edge_num_to_edge_map",
        ( std::unordered_map< int, PlacementTree::EdgeType * > ( * )( Sample const & ))( &::genesis::placement::edge_num_to_edge_map ),
        ( boost::python::arg("smp") ),
        get_docstring("std::unordered_map< int, PlacementTree::EdgeType * > ::genesis::placement::edge_num_to_edge_map (Sample const & smp)")
    );

    boost::python::def(
        "placements_per_edge",
        ( std::unordered_map< size_t, std::vector< PqueryPlacement const * > > ( * )( Sample const & ))( &::genesis::placement::placements_per_edge ),
        ( boost::python::arg("smp") ),
        get_docstring("std::unordered_map< size_t, std::vector< PqueryPlacement const * > > ::genesis::placement::placements_per_edge (Sample const & smp)")
    );

    boost::python::def(
        "placements_per_edge",
        ( std::vector< PqueryPlacement const * > ( * )( Sample const &, PlacementTreeEdge const & ))( &::genesis::placement::placements_per_edge ),
        ( boost::python::arg("smp"), boost::python::arg("edge") ),
        get_docstring("std::vector< PqueryPlacement const * > ::genesis::placement::placements_per_edge (Sample const & smp, PlacementTreeEdge const & edge)")
    );

    boost::python::def(
        "plain_queries",
        ( std::vector< PqueryPlain > ( * )( Sample const & ))( &::genesis::placement::plain_queries ),
        ( boost::python::arg("smp") ),
        get_docstring("std::vector< PqueryPlain > ::genesis::placement::plain_queries (Sample const & smp)")
    );
}
