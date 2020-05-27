QT       += network
QT       -= gui

TEMPLATE = lib

CONFIG += c++11

unix {
    QMAKE_CXXFLAGS += -Wno-format-nonliteral
    TARGET = KanoopCommon
    }

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += KANOOP_LIBRARY

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:
target.path = $$[QT_SYSROOT]/usr/local/lib

android: {
    target.path = /usr/local/lib/android
}

SOURCES += \
    addresshelper.cpp \
    jsonhelper.cpp \
    klog.cpp \
    mutexevent.cpp \
    pathutil.cpp \
    ratemonitor.cpp

HEADERS += \
    addresshelper.h \
    jsonhelper.h \
    kanoopcommon.h \
    klog.h \
    lockingqueue.h \
    mutexevent.h \
    pathutil.h \
    ratemonitor.h

header_files.files = $$HEADERS
header_files.path = $$[QT_SYSROOT]/usr/local/include/Kanoop
android: {
    header_files.path = /usr/local/include/android/Kanoop
}

message($$target.path);

INSTALLS = target
INSTALLS += header_files

