/*
 *  Author:
 *     Tai Tran <tai.tran@student.adelaide.edu.au>
 *          under the supervision of Guido Tack <guido.tack@monash.edu>
 */

#ifndef __PYINTERFACE_H
#define __PYINTERFACE_H

#include <Python.h>
#include "structmember.h"

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <list>
#include <string.h>
#include <typeinfo>
#include <cstdlib>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

#include <minizinc/parser.hh>
#include <minizinc/model.hh>
#include <minizinc/eval_par.hh>
#include <minizinc/typecheck.hh>
#include <minizinc/flatten.hh>
#include <minizinc/optimize.hh>
#include <minizinc/builtins.hh>
#include <minizinc/file_utils.hh>
#include <minizinc/solvers/gecode/gecode_solverinstance.hh>
#include <minizinc/prettyprinter.hh>

using namespace MiniZinc;
using namespace std;

// Error Objects
static PyObject* MznModel_new_error;
static PyObject* MznModel_init_error;
static PyObject* MznModel_solve_error;
static PyObject* MznModel_load_error;
static PyObject* MznModel_loadFromString_error;
static PyObject* MznModel_solve_warning;
static PyObject* MznVariable_init_error;
static PyObject* MznSet_error;

#include "MznSet.h"
#include "MznVariable.h"
//#include "MznConstraint.h"
//#include "MznSolveItem.h"
//#include "MznOutputItem.h"

// Time alarm
sigjmp_buf jmpbuf;

void sig_alrm (int signo)
{
  siglongjmp(jmpbuf, 1);
}

string minizinc_set(long start, long end);
int getList(PyObject* value, vector<Py_ssize_t>& dimensions, vector<PyObject*>& simpleArray, const int layer);


struct MznModel {
  PyObject_HEAD
  Model* _m;
  vector<string>* includePaths;

  unsigned int timeLimit;
  bool loaded;

  int load(PyObject *args, PyObject *keywds, bool fromFile);
  int addData(const char* const name, PyObject* value);
  PyObject* solve();
};

void MznModelDestructor(PyObject* o);
static PyObject* MznModel_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static int MznModel_init(MznModel* self, PyObject* args);
static void MznModel_dealloc(MznModel* self);

static PyObject* MznModel_load(MznModel *self, PyObject *args, PyObject *keywds);
static PyObject* MznModel_loadFromString(MznModel *self, PyObject *args, PyObject *keywds);
static PyObject* MznModel_solve(MznModel *self);
static PyObject* MznModel_setTimeLimit(MznModel* self, PyObject* args);
static PyObject* MznModel_addItem(MznModel* self, PyObject* args);
static PyObject* MznModel_addConstraint(MznModel* self, PyObject* args);
static PyObject* MznModel_Variable(MznModel* self, PyObject* args);
static PyObject* MznModel_Constraint(MznModel* self, PyObject* args);
static PyObject* MznModel_SolveItem(MznModel* self, PyObject* args);

static PyObject* Mzn_load(PyObject* self, PyObject* args, PyObject* keywds);
static PyObject* Mzn_loadFromString(PyObject* self, PyObject* args, PyObject* keywds);


static PyMemberDef MznModel_members[] = {
  {NULL} /* Sentinel */
};

static PyMethodDef MznModel_methods[] = {
  {"load", (PyCFunction)MznModel_load, METH_KEYWORDS, "Load MiniZinc model from MiniZinc file"},
  {"loadFromString", (PyCFunction)MznModel_loadFromString, METH_KEYWORDS, "Load MiniZinc model from standard input"},
  {"solve", (PyCFunction)MznModel_solve, METH_NOARGS, "Solve a loaded MiniZinc model"},
  {"setTimeLimit", (PyCFunction)MznModel_setTimeLimit, METH_VARARGS, "Limit the execution time of the model"},
  {"addItem", (PyCFunction)MznModel_addItem, METH_VARARGS, "Add items into the model"},
  {"Variable", (PyCFunction)MznModel_Variable, METH_VARARGS, "Add a variable into the model"},
  {"Constraint", (PyCFunction)MznModel_Constraint, METH_VARARGS, "Add a constraint into the model"},
  {"SolveItem", (PyCFunction)MznModel_SolveItem, METH_VARARGS, "Add a solve item into the model"},
  {NULL} /* Sentinel */
};

static PyMethodDef Mzn_methods[] = {
  {"load", (PyCFunction)Mzn_load, METH_KEYWORDS, "Load MiniZinc model from MiniZinc file"},
  {"loadFromString", (PyCFunction)Mzn_load, METH_KEYWORDS, "Load MiniZinc model from stdin"},
  {NULL}
};

static PyTypeObject MznModelType = {
  PyVarObject_HEAD_INIT(NULL,0)
  "minizinc.Model",       /* tp_name */
  sizeof(MznModel),          /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)MznModel_dealloc, /* tp_dealloc */
  0,                         /* tp_print */
  0,                         /* tp_getattr */
  0,                         /* tp_setattr */
  0,                         /* tp_reserved */
  0,                         /* tp_repr */
  0,                         /* tp_as_number */
  0,                         /* tp_as_sequence */
  0,                         /* tp_as_mapping */
  0,                         /* tp_hash  */
  0,                         /* tp_call */
  0,                         /* tp_str */
  0,                         /* tp_getattro */
  0,                         /* tp_setattro */
  0,                         /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
  "Minizinc Model",  /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  MznModel_methods,            /* tp_methods */
  MznModel_members,            /* tp_members */
  0,/*MznModel_getseters,        /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)MznModel_init,     /* tp_init */
  0,                         /* tp_alloc */
  MznModel_new,                /* tp_new */
};




struct MznSolution {
  PyObject_HEAD
  Env* env;
  SolverInstance::Status status;

  PyObject* next();
};

static PyObject* MznSolution_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static int MznSolution_init(MznSolution* self, PyObject* args);
static void MznSolution_dealloc(MznSolution* self);

static PyObject* MznSolution_next(MznSolution *self);



static PyMemberDef MznSolution_members[] = {
  {NULL} /* Sentinel */
};

static PyMethodDef MznSolution_methods[] = {
  {"next", (PyCFunction)MznSolution_next, METH_KEYWORDS, "Next solution"},
  {NULL} /* Sentinel */
};


static PyTypeObject MznSolutionType = {
  PyVarObject_HEAD_INIT(NULL,0)
  "minizinc.model",       /* tp_name */
  sizeof(MznSolution),          /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)MznSolution_dealloc, /* tp_dealloc */
  0,                         /* tp_print */
  0,                         /* tp_getattr */
  0,                         /* tp_setattr */
  0,                         /* tp_reserved */
  0,                         /* tp_repr */
  0,                         /* tp_as_number */
  0,                         /* tp_as_sequence */
  0,                         /* tp_as_mapping */
  0,                         /* tp_hash  */
  0,                         /* tp_call */
  0,                         /* tp_str */
  0,                         /* tp_getattro */
  0,                         /* tp_setattro */
  0,                         /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
  "Minizinc Solution",  /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  MznSolution_methods,            /* tp_methods */
  MznSolution_members,            /* tp_members */
  0,/*MznSolution_getseters,        /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)MznSolution_init,     /* tp_init */
  0,                         /* tp_alloc */
  MznSolution_new,                /* tp_new */
};

PyMODINIT_FUNC
initminizinc(void) {
    GC::init();
    PyObject* model = Py_InitModule3("minizinc", Mzn_methods, "A python interface for minizinc constraint modeling");

    if (model == NULL)
      return;

    //Error Handling
    MznModel_load_error = PyErr_NewException("MznModel_load.error", NULL, NULL);
    if (MznModel_load_error == NULL)
      return;
    Py_INCREF(MznModel_load_error);
    PyModule_AddObject(model,"error",MznModel_load_error);

    MznModel_loadFromString_error = PyErr_NewException("MznModel_loadFromString.error", NULL, NULL);
    if (MznModel_loadFromString_error == NULL)
      return;
    Py_INCREF(MznModel_loadFromString_error);
    PyModule_AddObject(model,"error",MznModel_loadFromString_error);


    MznModel_solve_error = PyErr_NewException("MznModel_solve.error", NULL, NULL);
    if (MznModel_solve_error == NULL)
      return;
    Py_INCREF(MznModel_solve_error);
    PyModule_AddObject(model,"error",MznModel_solve_error);

    MznModel_solve_warning = PyErr_NewException("MznModel_solve.warning", NULL, NULL);
    if (MznModel_solve_error == NULL)
      return;
    Py_INCREF(MznModel_solve_warning);
    PyModule_AddObject(model,"warning",MznModel_solve_warning);

    MznSet_error = PyErr_NewException("MznSet.error", NULL, NULL);
    if (MznSet_error == NULL)
      return;
    Py_INCREF(MznSet_error);
    PyModule_AddObject(model,"error",MznSet_error);

    MznVariable_init_error = PyErr_NewException("MznVariable_init.error", NULL, NULL);
    if (MznVariable_init_error == NULL)
      return;
    Py_INCREF(MznVariable_init_error);
    PyModule_AddObject(model,"error",MznVariable_init_error);


    // Minizinc Set Initialization
    if (PyType_Ready(&MznSetType) < 0)
      return;
    Py_INCREF(&MznSetType);
    PyModule_AddObject(model, "Set", (PyObject *)&MznSetType);

    if (PyType_Ready(&MznVariableType) < 0)
      return;
    Py_INCREF(&MznVariableType);
    PyModule_AddObject(model, "Variable", (PyObject *)&MznVariableType);

    /*if (PyType_Ready(&MznConstraintType) < 0)
      return;
    Py_INCREF(&MznConstraintType);
    PyModule_AddObject(model, "Constraint", (PyObject *)&MznConstraintType);*/

    if (PyType_Ready(&MznModelType) < 0)
      return;
    Py_INCREF(&MznModelType);
    PyModule_AddObject(model, "Model", (PyObject *)&MznModelType);

    if (PyType_Ready(&MznSolutionType) < 0)
      return;
    Py_INCREF(&MznSolutionType);
    PyModule_AddObject(model, "Solution", (PyObject *)&MznSolutionType);

    /*if (PyType_Ready(&MznSolveItemType) < 0)
      return;
    Py_INCREF(&MznSolveItemType);
    PyModule_AddObject(model, "Solve", (PyObject *)&MznSolveItemType);*/

    /*if (PyType_Ready(&MznOutputItemType) < 0)
      return;
    Py_INCREF(&MznOutputItemType);
    PyModule_AddObject(model, "Output", (PyObject *)&MznOutputItemType);*/
}

#endif