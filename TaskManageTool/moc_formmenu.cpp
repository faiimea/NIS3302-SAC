/****************************************************************************
** Meta object code from reading C++ file 'formmenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "formmenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FormMenu_t {
    QByteArrayData data[15];
    char stringdata0[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FormMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FormMenu_t qt_meta_stringdata_FormMenu = {
    {
QT_MOC_LITERAL(0, 0, 8), // "FormMenu"
QT_MOC_LITERAL(1, 9, 20), // "sendAddButtonPressed"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "QMap<int,task>*"
QT_MOC_LITERAL(4, 47, 1), // "m"
QT_MOC_LITERAL(5, 49, 13), // "TimerResponse"
QT_MOC_LITERAL(6, 63, 24), // "on_pushButtonAdd_clicked"
QT_MOC_LITERAL(7, 88, 27), // "on_pushButtonDelete_clicked"
QT_MOC_LITERAL(8, 116, 23), // "RecvSignInButtonPressed"
QT_MOC_LITERAL(9, 140, 7), // "strUser"
QT_MOC_LITERAL(10, 148, 10), // "RecvSearch"
QT_MOC_LITERAL(11, 159, 4), // "task"
QT_MOC_LITERAL(12, 164, 1), // "t"
QT_MOC_LITERAL(13, 166, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(14, 188, 23) // "on_pushButton_2_clicked"

    },
    "FormMenu\0sendAddButtonPressed\0\0"
    "QMap<int,task>*\0m\0TimerResponse\0"
    "on_pushButtonAdd_clicked\0"
    "on_pushButtonDelete_clicked\0"
    "RecvSignInButtonPressed\0strUser\0"
    "RecvSearch\0task\0t\0on_pushButton_clicked\0"
    "on_pushButton_2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FormMenu[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    1,   60,    2, 0x08 /* Private */,
      10,    1,   63,    2, 0x08 /* Private */,
      13,    0,   66,    2, 0x08 /* Private */,
      14,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FormMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FormMenu *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendAddButtonPressed((*reinterpret_cast< QMap<int,task>*(*)>(_a[1]))); break;
        case 1: _t->TimerResponse(); break;
        case 2: _t->on_pushButtonAdd_clicked(); break;
        case 3: _t->on_pushButtonDelete_clicked(); break;
        case 4: _t->RecvSignInButtonPressed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->RecvSearch((*reinterpret_cast< task(*)>(_a[1]))); break;
        case 6: _t->on_pushButton_clicked(); break;
        case 7: _t->on_pushButton_2_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FormMenu::*)(QMap<int,task> * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FormMenu::sendAddButtonPressed)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FormMenu::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_FormMenu.data,
    qt_meta_data_FormMenu,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FormMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FormMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FormMenu.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FormMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void FormMenu::sendAddButtonPressed(QMap<int,task> * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
