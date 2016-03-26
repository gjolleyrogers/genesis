/**
 * @brief
 *
 * @file
 * @ingroup python
 */

#include <python/src/common.hpp>

#include "lib/genesis.hpp"

template <typename T>
void PythonExportClass_Matrix(std::string name)
{

    // -------------------------------------------------------------------
    //     Class Matrix
    // -------------------------------------------------------------------

    using namespace ::genesis::utils;

    using MatrixType = Matrix<typename T>;

    boost::python::class_< MatrixType > ( name.c_str(), boost::python::init< size_t, size_t >(( boost::python::arg("rows"), boost::python::arg("cols") )) )
        .def( boost::python::init< size_t, size_t, T >(( boost::python::arg("rows"), boost::python::arg("cols"), boost::python::arg("init") )) )
        .def( boost::python::init< size_t, size_t, std::initializer_list< T > const & >(( boost::python::arg("rows"), boost::python::arg("cols"), boost::python::arg("init_list") )) )
        .def( boost::python::init< Matrix const & >(( boost::python::arg("") )) )

        // Public Member Functions

        .def(
            "at",
            ( T & ( MatrixType::* )( const size_t, const size_t ))( &MatrixType::at ),
            ( boost::python::arg("row"), boost::python::arg("col") ),
            boost::python::return_value_policy<boost::python::reference_existing_object>()
        )
        .def(
            "at",
            ( const T ( MatrixType::* )( const size_t, const size_t ) const )( &MatrixType::at ),
            ( boost::python::arg("row"), boost::python::arg("col") )
        )
        .def(
            "cbegin",
            ( const_iterator ( MatrixType::* )(  ) const )( &MatrixType::cbegin )
        )
        .def(
            "cend",
            ( const_iterator ( MatrixType::* )(  ) const )( &MatrixType::cend )
        )
        .def(
            "cols",
            ( size_t ( MatrixType::* )(  ) const )( &MatrixType::cols )
        )
        .def(
            "rows",
            ( size_t ( MatrixType::* )(  ) const )( &MatrixType::rows )
        )
        .def(
            "size",
            ( size_t ( MatrixType::* )(  ) const )( &MatrixType::size )
        )
        .def(
            "swap",
            ( void ( MatrixType::* )( Matrix & ))( &MatrixType::swap ),
            ( boost::python::arg("other") )
        )

        // Operators

        .def( boost::python::self != boost::python::self )
        .def( boost::python::self == boost::python::self )

        // Iterators

        .def(
            "__iter__",
            boost::python::range ( &MatrixType::begin, &MatrixType::end )
        )
    ;
}
