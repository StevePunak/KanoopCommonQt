QT       += network
QT       -= gui

TEMPLATE = lib

CONFIG += c++11

unix {
    QMAKE_CXXFLAGS += -Wno-format-nonliteral -Wno-format-security
    TARGET = KanoopCommon
    }

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += KANOOP_LIBRARY

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# load qmake variables which need to be brought in from from environment
OUTPUT_PREFIX=$$(OUTPUT_PREFIX)
!isEmpty(OUTPUT_PREFIX) {
    OUTPUT_PREFIX=$$OUTPUT_PREFIX/target/$$basename(QMAKESPEC)
}

# If QT_BUILD_OUTPUT_ROOT is set, we will output to (QT_SYSROOT)/(OUTPUT_PREFIX)/usr/local/lib
# Otherwise, it's (QT_SYSROOT)/usr/local/lib
!android {
    target.path = $$OUTPUT_PREFIX/usr/local/lib
} else {
    target.path = /usr/local/lib/android
}

SOURCES += \
    addresshelper.cpp \
    datetimeutil.cpp \
    jsonhelper.cpp \
    klog.cpp \
    mutexevent.cpp \
    pathutil.cpp \
    ratemonitor.cpp

HEADERS += \
    addresshelper.h \
    datetimeutil.h \
    jsonhelper.h \
    kanoopcommon.h \
    klog.h \
    lockingqueue.h \
    mutexevent.h \
    pathutil.h \
    ratemonitor.h

header_files.files = $$HEADERS
header_files.path = $$OUTPUT_PREFIX/usr/local/include/Kanoop
android: {
    header_files.path = /usr/local/include/android/Kanoop
}

INSTALLS = target
INSTALLS += header_files

QMAKE_POST_LINK=$(MAKE) install
