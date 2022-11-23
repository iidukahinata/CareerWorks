/**
* @file    ModuleHelper.cpp
* @brief
*
* @date	   2022/11/13 2022îNìxèâî≈
*/
#pragma once


#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include "Subsystem/Core/Common/pch.h"

#define PY_MAKE_FUNC(FUNC, POLICIES)	boost::python::make_function(FUNC, POLICIES)

#define PY_ENUM(ENUM)	boost::python::enum_<ENUM>(#ENUM)
#define PY_VALUE(VALUE) .value(#VALUE, VALUE)

#define PY_CLASS(CLASS)					  boost::python::class_<CLASS>(#CLASS)
#define PY_CLASS_INIT(CLASS, ARGS)		  boost::python::class_<CLASS>(#CLASS, py::init<ARGS>())
#define PY_CLASS_NO_INIT(CLASS)			  boost::python::class_<CLASS>(#CLASS, py::no_init)
#define PY_CLASS_NOCOPY(CLASS)			  boost::python::class_<CLASS, boost::noncopyable>(#CLASS)
#define PY_CLASS_NOCOPY_NO_INIT(CLASS)	  boost::python::class_<CLASS, boost::noncopyable>(#CLASS, py::no_init)
#define PY_BASE_CLASS(CLASS, BASE)		  boost::python::class_<CLASS, boost::python::bases<BASE>>(#CLASS)
#define PY_BASE_CLASS_NOCOPY(CLASS, BASE) boost::python::class_<CLASS, boost::python::bases<BASE>, boost::noncopyable>(#CLASS)

#define PY_DEF(FUNC)										 boost::python::def(#FUNC, &FUNC)
#define PY_DEF_POLICIE(FUNC, POLICIE)						 boost::python::def(#FUNC, &FUNC, POLICIE)
#define PY_CLASS_DEF(CLASS, FUNC)							 .def(#FUNC, &CLASS::FUNC)
#define PY_CLASS_DEF_INIT(ARGS)								 .def(boost::python::init<ARGS>())
#define PY_CLASS_DEF_POLICIE(CLASS, FUNC, POLICIE)			 .def(#FUNC, &CLASS::FUNC, POLICIE)
#define PY_CLASS_DEF_OVERLOADS(CLASS, FUNC, OVERLOADS)		 .def(#FUNC, &CLASS::FUNC, OVERLOADS)
#define PY_CLASS_GET(NAME, CLASS, FUNC)						 .add_property(NAME, &CLASS::FUNC)
#define PY_CLASS_MAKE_GET(NAME, CLASS, FUNC, POLICIE)		 .add_property(NAME, PY_MAKE_FUNC(&CLASS::FUNC, POLICIE))
#define PY_CLASS_GETSET(NAME, CLASS, GET, SET)				 .add_property(NAME, &CLASS::GET, &CLASS::SET)
#define PY_CLASS_MAKE_GETSET(NAME, CLASS, GET, POLICIE, SET) .add_property(NAME, PY_MAKE_FUNC(&CLASS::GET, POLICIE), &CLASS::SET)

#define PY_READ_ONLY(CLASS, VALUE)  .def_readonly(#VALUE, &CLASS::VALUE)
#define PY_READ_WRITE(CLASS, VALUE) .def_readwrite(#VALUE, &CLASS::VALUE)

#define PY_OPAQUE_PTR     boost::python::return_value_policy<boost::python::return_opaque_pointer>()
#define PY_BY_VALUE       boost::python::return_value_policy<boost::python::return_by_value>()
#define PY_COPY_CONST_REF boost::python::return_value_policy<boost::python::copy_const_reference>()
#define PY_NOT_CONST_REF  boost::python::return_value_policy<boost::python::copy_non_const_reference>()
#define PY_RET_REF		  boost::python::return_internal_reference<>()


void SetUpCoreModule();
void SetUpMathModule();
void SetUpInputModule();
void SetUpResourceModule();
void SetUpSceneModule();
void SetUpComponentModule();

void InitModule();