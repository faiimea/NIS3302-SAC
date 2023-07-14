/****************************************************************************
** Meta object code from reading C++ file 'mainwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWidget_t {
    QByteArrayData data[16];
    char stringdata0[275];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWidget_t qt_meta_stringdata_MainWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWidget"
QT_MOC_LITERAL(1, 11, 19), // "SendOldUserPassword"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 25), // "QMap<QString,QByteArray>*"
QT_MOC_LITERAL(4, 58, 1), // "m"
QT_MOC_LITERAL(5, 60, 20), // "SendCreateNewAccount"
QT_MOC_LITERAL(6, 81, 23), // "SendSignInButtonPressed"
QT_MOC_LITERAL(7, 105, 7), // "strUser"
QT_MOC_LITERAL(8, 113, 27), // "on_pushButtonSignIn_clicked"
QT_MOC_LITERAL(9, 141, 27), // "on_pushButtonSignUp_clicked"
QT_MOC_LITERAL(10, 169, 27), // "on_pushButtonChange_clicked"
QT_MOC_LITERAL(11, 197, 19), // "RecvNewUserPassword"
QT_MOC_LITERAL(12, 217, 11), // "strUsername"
QT_MOC_LITERAL(13, 229, 11), // "strPassword"
QT_MOC_LITERAL(14, 241, 11), // "newPassword"
QT_MOC_LITERAL(15, 253, 21) // "RecvNewAccountCreated"

    },
    "MainWidget\0SendOldUserPassword\0\0"
    "QMap<QString,QByteArray>*\0m\0"
    "SendCreateNewAccount\0SendSignInButtonPressed\0"
    "strUser\0on_pushButtonSignIn_clicked\0"
    "on_pushButtonSignUp_clicked\0"
    "on_pushButtonChange_clicked\0"
    "RecvNewUserPassword\0strUsername\0"
    "strPassword\0newPassword\0RecvNewAccountCreated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   63,    2, 0x08 /* Private */,
       9,    0,   64,    2, 0x08 /* Private */,
      10,    0,   65,    2, 0x08 /* Private */,
      11,    3,   66,    2, 0x08 /* Private */,
      15,    2,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   12,   13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   12,   13,

       0        // eod
};

void MainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->SendOldUserPassword((*reinterpret_cast< QMap<QString,QByteArray>*(*)>(_a[1]))); break;
        case 1: _t->SendCreateNewAccount((*reinterpret_cast< QMap<QString,QByteArray>*(*)>(_a[1]))); break;
        case 2: _t->SendSignInButtonPressed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->on_pushButtonSignIn_clicked(); break;
        case 4: _t->on_pushButtonSignUp_clicked(); break;
        case 5: _t->on_pushButtonChange_clicked(); break;
        case 6: _t->RecvNewUserPassword((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->RecvNewAccountCreated((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWidget::*)(QMap<QString,QByteArray> * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWidget::SendOldUserPassword)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)(QMap<QString,QByteArray> * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWidget::SendCreateNewAccount)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWidget::SendSignInButtonPressed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainWidget.data,
    qt_meta_data_MainWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void MainWidget::SendOldUserPassword(QMap<QString,QByteArray> * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWidget::SendCreateNewAccount(QMap<QString,QByteArray> * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWidget::SendSignInButtonPressed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
