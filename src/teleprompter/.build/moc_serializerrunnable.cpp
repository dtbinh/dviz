/****************************************************************************
** Meta object code from reading C++ file 'serializerrunnable.h'
**
** Created: Sat Dec 19 18:15:34 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../3rdparty/qjson/serializerrunnable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serializerrunnable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QJson__SerializerRunnable[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      51,   27,   26,   26, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QJson__SerializerRunnable[] = {
    "QJson::SerializerRunnable\0\0"
    "serialized,ok,error_msg\0"
    "parsingFinished(QByteArray,bool,QString)\0"
};

const QMetaObject QJson::SerializerRunnable::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QJson__SerializerRunnable,
      qt_meta_data_QJson__SerializerRunnable, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QJson::SerializerRunnable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QJson::SerializerRunnable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QJson::SerializerRunnable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QJson__SerializerRunnable))
        return static_cast<void*>(const_cast< SerializerRunnable*>(this));
    if (!strcmp(_clname, "QRunnable"))
        return static_cast< QRunnable*>(const_cast< SerializerRunnable*>(this));
    return QObject::qt_metacast(_clname);
}

int QJson::SerializerRunnable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: parsingFinished((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QJson::SerializerRunnable::parsingFinished(const QByteArray & _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
