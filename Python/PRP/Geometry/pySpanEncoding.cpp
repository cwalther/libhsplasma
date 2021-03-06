/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <PyPlasma.h>
#include <PRP/Geometry/plSpanInstance.h>
#include "pySpanInstance.h"
#include "Stream/pyStream.h"

extern "C" {

static void pySpanEncoding_dealloc(pySpanEncoding* self) {
    if (self->fPyOwned)
        delete self->fThis;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static int pySpanEncoding___init__(pySpanEncoding* self, PyObject* args, PyObject* kwds) {
    if (!PyArg_ParseTuple(args, ""))
        return -1;
    return 0;
}

static PyObject* pySpanEncoding_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pySpanEncoding* self = (pySpanEncoding*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->fThis = new plSpanEncoding();
        self->fPyOwned = true;
    }
    return (PyObject*)self;
}

static PyObject* pySpanEncoding_read(pySpanEncoding* self, PyObject* args) {
    pyStream* stream;
    if (!PyArg_ParseTuple(args, "O", &stream)) {
        PyErr_SetString(PyExc_TypeError, "read expects a hsStream");
        return NULL;
    }
    if (!pyStream_Check((PyObject*)stream)) {
        PyErr_SetString(PyExc_TypeError, "read expects a hsStream");
        return NULL;
    }
    self->fThis->read(stream->fThis);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pySpanEncoding_write(pySpanEncoding* self, PyObject* args) {
    pyStream* stream;
    if (!PyArg_ParseTuple(args, "O", &stream)) {
        PyErr_SetString(PyExc_TypeError, "write expects a hsStream");
        return NULL;
    }
    if (!pyStream_Check((PyObject*)stream)) {
        PyErr_SetString(PyExc_TypeError, "write expects a hsStream");
        return NULL;
    }
    self->fThis->write(stream->fThis);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pySpanEncoding_getCode(pySpanEncoding* self, void*) {
    return PyInt_FromLong(self->fThis->getCode());
}

static PyObject* pySpanEncoding_getPosScale(pySpanEncoding* self, void*) {
    return PyFloat_FromDouble(self->fThis->getPosScale());
}

static int pySpanEncoding_setCode(pySpanEncoding* self, PyObject* value, void*) {
    if (value == NULL || !PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "code should be an int");
        return -1;
    }
    self->fThis->setCode(PyInt_AsLong(value));
    return 0;
}

static int pySpanEncoding_setPosScale(pySpanEncoding* self, PyObject* value, void*) {
    if (value == NULL || !PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "posScale should be a float");
        return -1;
    }
    self->fThis->setPosScale(PyFloat_AsDouble(value));
    return 0;
}

static PyMethodDef pySpanEncoding_Methods[] = {
    { "read", (PyCFunction)pySpanEncoding_read, METH_VARARGS,
      "Params: stream\n"
      "Reads this object from the stream" },
    { "write", (PyCFunction)pySpanEncoding_write, METH_VARARGS,
      "Params: stream\n"
      "Writes this object to the stream" },
    { NULL, NULL, 0, NULL }
};

static PyGetSetDef pySpanEncoding_GetSet[] = {
    { _pycs("code"), (getter)pySpanEncoding_getCode,
        (setter)pySpanEncoding_setCode, NULL, NULL },
    { _pycs("posScale"), (getter)pySpanEncoding_getPosScale,
        (setter)pySpanEncoding_setPosScale, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pySpanEncoding_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyPlasma.plSpanEncoding",          /* tp_name */
    sizeof(pySpanEncoding),             /* tp_basicsize */
    0,                                  /* tp_itemsize */

    (destructor)pySpanEncoding_dealloc, /* tp_dealloc */
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

    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    "plSpanEncoding wrapper",           /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pySpanEncoding_Methods,             /* tp_methods */
    NULL,                               /* tp_members */
    pySpanEncoding_GetSet,              /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    (initproc)pySpanEncoding___init__,  /* tp_init */
    NULL,                               /* tp_alloc */
    pySpanEncoding_new,                 /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */

    NULL,                               /* tp_del */
    TP_VERSION_TAG_INIT                 /* tp_version_tag */
};

PyObject* Init_pySpanEncoding_Type() {
    if (PyType_Ready(&pySpanEncoding_Type) < 0)
        return NULL;

    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kPosNone",
                         PyInt_FromLong(plSpanEncoding::kPosNone));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kPos888",
                         PyInt_FromLong(plSpanEncoding::kPos888));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kPos161616",
                         PyInt_FromLong(plSpanEncoding::kPos161616));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kPos101010",
                         PyInt_FromLong(plSpanEncoding::kPos101010));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kPos008",
                         PyInt_FromLong(plSpanEncoding::kPos008));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kPosMask",
                         PyInt_FromLong(plSpanEncoding::kPosMask));

    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kColNone",
                         PyInt_FromLong(plSpanEncoding::kColNone));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kColA8",
                         PyInt_FromLong(plSpanEncoding::kColA8));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kColI8",
                         PyInt_FromLong(plSpanEncoding::kColI8));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kColAI88",
                         PyInt_FromLong(plSpanEncoding::kColAI88));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kColRGB888",
                         PyInt_FromLong(plSpanEncoding::kColRGB888));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kColARGB8888",
                         PyInt_FromLong(plSpanEncoding::kColARGB8888));
    PyDict_SetItemString(pySpanEncoding_Type.tp_dict, "kColMask",
                         PyInt_FromLong(plSpanEncoding::kColMask));

    Py_INCREF(&pySpanEncoding_Type);
    return (PyObject*)&pySpanEncoding_Type;
}

int pySpanEncoding_Check(PyObject* obj) {
    if (obj->ob_type == &pySpanEncoding_Type
        || PyType_IsSubtype(obj->ob_type, &pySpanEncoding_Type))
        return 1;
    return 0;
}

PyObject* pySpanEncoding_FromSpanEncoding(plSpanEncoding* encoding) {
    if (encoding == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    pySpanEncoding* obj = PyObject_New(pySpanEncoding, &pySpanEncoding_Type);
    obj->fThis = encoding;
    obj->fPyOwned = false;
    return (PyObject*)obj;
}

}
