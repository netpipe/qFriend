/****************************************************************************
** Meta object code from reading C++ file 'audiorecorder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "audiorecorder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiorecorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AudioRecorder_t {
    QByteArrayData data[23];
    char stringdata0[400];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioRecorder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioRecorder_t qt_meta_stringdata_AudioRecorder = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AudioRecorder"
QT_MOC_LITERAL(1, 14, 13), // "processBuffer"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 12), // "QAudioBuffer"
QT_MOC_LITERAL(4, 42, 17), // "setOutputLocation"
QT_MOC_LITERAL(5, 60, 11), // "togglePause"
QT_MOC_LITERAL(6, 72, 12), // "toggleRecord"
QT_MOC_LITERAL(7, 85, 12), // "updateStatus"
QT_MOC_LITERAL(8, 98, 22), // "QMediaRecorder::Status"
QT_MOC_LITERAL(9, 121, 14), // "onStateChanged"
QT_MOC_LITERAL(10, 136, 21), // "QMediaRecorder::State"
QT_MOC_LITERAL(11, 158, 14), // "updateProgress"
QT_MOC_LITERAL(12, 173, 3), // "pos"
QT_MOC_LITERAL(13, 177, 19), // "displayErrorMessage"
QT_MOC_LITERAL(14, 197, 13), // "iconActivated"
QT_MOC_LITERAL(15, 211, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(16, 245, 6), // "reason"
QT_MOC_LITERAL(17, 252, 45), // "on_horizontalSlider_thresoldl..."
QT_MOC_LITERAL(18, 298, 8), // "position"
QT_MOC_LITERAL(19, 307, 31), // "on_horizontalSlider_sliderMoved"
QT_MOC_LITERAL(20, 339, 12), // "AudioRestart"
QT_MOC_LITERAL(21, 352, 13), // "StopRecording"
QT_MOC_LITERAL(22, 366, 33) // "on_horizontalSlider_2_sliderM..."

    },
    "AudioRecorder\0processBuffer\0\0QAudioBuffer\0"
    "setOutputLocation\0togglePause\0"
    "toggleRecord\0updateStatus\0"
    "QMediaRecorder::Status\0onStateChanged\0"
    "QMediaRecorder::State\0updateProgress\0"
    "pos\0displayErrorMessage\0iconActivated\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0on_horizontalSlider_thresoldlevel_sliderMoved\0"
    "position\0on_horizontalSlider_sliderMoved\0"
    "AudioRestart\0StopRecording\0"
    "on_horizontalSlider_2_sliderMoved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioRecorder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x0a /* Public */,
       4,    0,   87,    2, 0x08 /* Private */,
       5,    0,   88,    2, 0x08 /* Private */,
       6,    0,   89,    2, 0x08 /* Private */,
       7,    1,   90,    2, 0x08 /* Private */,
       9,    1,   93,    2, 0x08 /* Private */,
      11,    1,   96,    2, 0x08 /* Private */,
      13,    0,   99,    2, 0x08 /* Private */,
      14,    1,  100,    2, 0x08 /* Private */,
      17,    1,  103,    2, 0x08 /* Private */,
      19,    1,  106,    2, 0x08 /* Private */,
      20,    0,  109,    2, 0x08 /* Private */,
      21,    0,  110,    2, 0x08 /* Private */,
      22,    1,  111,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, QMetaType::LongLong,   12,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void AudioRecorder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AudioRecorder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->processBuffer((*reinterpret_cast< const QAudioBuffer(*)>(_a[1]))); break;
        case 1: _t->setOutputLocation(); break;
        case 2: _t->togglePause(); break;
        case 3: _t->toggleRecord(); break;
        case 4: _t->updateStatus((*reinterpret_cast< QMediaRecorder::Status(*)>(_a[1]))); break;
        case 5: _t->onStateChanged((*reinterpret_cast< QMediaRecorder::State(*)>(_a[1]))); break;
        case 6: _t->updateProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 7: _t->displayErrorMessage(); break;
        case 8: _t->iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 9: _t->on_horizontalSlider_thresoldlevel_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_horizontalSlider_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->AudioRestart(); break;
        case 12: _t->StopRecording(); break;
        case 13: _t->on_horizontalSlider_2_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaRecorder::Status >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaRecorder::State >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AudioRecorder::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_AudioRecorder.data,
    qt_meta_data_AudioRecorder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AudioRecorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioRecorder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AudioRecorder.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AudioRecorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
