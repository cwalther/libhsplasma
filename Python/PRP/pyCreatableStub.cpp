#include <Python.h>
#include <PRP/plCreatable.h>
#include "pyCreatable.h"

extern "C" {

static void pyCreatableStub_dealloc(pyCreatable* self) {
    if (self->fThis != NULL)
        delete self->fThis;
    self->ob_type->tp_free((PyObject*)self);
}

static int pyCreatableStub___init__(pyCreatableStub* self, PyObject* args, PyObject* kwds) {
    int classId, length;
    if (!PyArg_ParseTuple(args, "ii", &classId, &length)) {
        PyErr_SetString(PyExc_TypeError, "__init__ expects int, int");
        return -1;
    }
    if (self->fThis != NULL)
        delete self->fThis;
    self->fThis = new plCreatableStub(classId, (size_t)length);
    return 0;
}

static PyObject* pyCreatableStub_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyCreatableStub* self = (pyCreatableStub*)type->tp_alloc(type, 0);
    self->fThis = NULL;
    return (PyObject*)self;
}

static PyObject* pyCreatableStub_getClassIdx(pyCreatableStub* self) {
    return PyInt_FromLong(self->fThis->getClassIdx());
}

static PyObject* pyCreatableStub_getData(pyCreatableStub* self) {
    return PyString_FromStringAndSize((const char*)self->fThis->getData(),
                                      self->fThis->getLength());
}

static PyObject* pyCreatableStub_getLength(pyCreatableStub* self) {
    return PyInt_FromLong(self->fThis->getLength());
}

static PyMethodDef pyCreatableStub_Methods[] = {
    { "getClassIdx", (PyCFunction)pyCreatableStub_getClassIdx, METH_NOARGS,
      "Returns the Stub's Class Index" },
    { "getData", (PyCFunction)pyCreatableStub_getData, METH_NOARGS,
      "Returns the Creatable's raw data" },
    { "getLength", (PyCFunction)pyCreatableStub_getLength, METH_NOARGS,
      "Returns the length of the Creatable's raw data" },
    { NULL, NULL, 0, NULL }
};

PyTypeObject pyCreatableStub_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "PyPlasma.plCreatableStub",         /* tp_name */
    sizeof(pyCreatableStub),            /* tp_basicsize */
    0,                                  /* tp_itemsize */

    (destructor)pyCreatableStub_dealloc, /* tp_dealloc */
    NULL,                               /* tp_print */
    NULL,                               /* tp_getattr */
    NULL,                               /* tp_setattr */
    NULL,                               /* tp_compare */
    NULL,                               /* tp_repr */
    NULL,                               /* tp_as_number */
    NULL,                               /* tp_as_sequence */
    NULL,                               /* tp_as_mapping */
    NULL,                               /* tp_hash */
    NULL,                               /* tp_call */
    NULL,                               /* tp_str */
    NULL,                               /* tp_getattro */
    NULL,                               /* tp_setattro */
    NULL,                               /* tp_as_buffer */

    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "plCreatableStub wrapper",          /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyCreatableStub_Methods,            /* tp_methods */
    NULL,                               /* tp_members */
    NULL,                               /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    (initproc)pyCreatableStub___init__, /* tp_init */
    NULL,                               /* tp_alloc */
    pyCreatableStub_new,                /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */
};

PyObject* Init_pyCreatableStub_Type() {
    pyCreatableStub_Type.tp_base = &pyCreatable_Type;
    if (PyType_Ready(&pyCreatableStub_Type) < 0)
        return NULL;

    Py_INCREF(&pyCreatableStub_Type);
    return (PyObject*)&pyCreatableStub_Type;
}

}