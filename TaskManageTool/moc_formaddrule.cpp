/****************************************************************************
** Meta object code from reading C++ file 'formaddrule.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "formaddrule.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formaddrule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FormAddRule_t {
    QByteArrayData data[10];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FormAddRule_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FormAddRule_t qt_meta_stringdata_FormAddRule = {
    {
QT_MOC_LITERAL(0, 0, 11), // "FormAddRule"
QT_MOC_LITERAL(1, 12, 10), // "sendSearch"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 4), // "task"
QT_MOC_LITERAL(4, 29, 1), // "t"
QT_MOC_LITERAL(5, 31, 23), // "on_pushButtonOK_clicked"
QT_MOC_LITERAL(6, 55, 20), // "RecvAddButtonPressed"
QT_MOC_LITERAL(7, 76, 15), // "QMap<int,task>*"
QT_MOC_LITERAL(8, 92, 1), // "m"
QT_MOC_LITERAL(9, 94, 21) // "on_pushButton_clicked"

    },
    "FormAddRule\0sendSearch\0\0task\0t\0"
    "on_pushButtonOK_clicked\0RecvAddButtonPressed\0"
    "QMap<int,task>*\0m\0on_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FormAddRule[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x08 /* Private */,
       6,    1,   38,    2, 0x08 /* Private */,
       9,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

       0        // eod
};

void FormAddRule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FormAddRule *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendSearch((*reinterpret_cast< task(*)>(_a[1]))); break;
        case 1: _t->on_pushButtonOK_clicked(); break;
        case 2: _t->RecvAddButtonPressed((*reinterpret_cast< QMap<int,task>*(*)>(_a[1]))); break;
        case 3: _t->on_pushButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FormAddRule::*)(task );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FormAddRule::sendSearch)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FormAddRule::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_FormAddRule.data,
    qt_meta_data_FormAddRule,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FormAddRule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FormAddRule::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FormAddRule.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FormAddRule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FormAddRule::sendSearch(task _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
