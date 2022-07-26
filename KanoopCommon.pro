QT       += network
QT       -= gui

TEMPLATE = lib
VERSION=1.0.13

CONFIG += c++11

unix {
#    QMAKE_CXXFLAGS += -Wno-format-nonliteral -Wno-format-security -O2
    TARGET = KanoopCommon
}

win32 {
    CONFIG += skip_target_version_ext
}

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += KANOOP_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# load qmake variables which need to be brought in from from environment
OUTPUT_PREFIX=$$(OUTPUT_PREFIX)
IS_TARGET_BUILD=$$(IS_TARGET_BUILD)

if(!isEmpty(OUTPUT_PREFIX):isEmpty(IS_TARGET_BUILD)) {
    OUTPUT_PREFIX=$$OUTPUT_PREFIX/target/$$basename(QMAKESPEC)
}

target.path = $$OUTPUT_PREFIX/usr/lib

SOURCES += \
    addresshelper.cpp \
    cryptoutil.cpp \
    datetimeutil.cpp \
    geo/earthgeo.cpp \
    fileutil.cpp \
    geo/geocircle.cpp \
    geo/geocoordinate.cpp \
    geo/geotypes.cpp \
    jsonhelper.cpp \
    klog.cpp \
    mutexevent.cpp \
    pathutil.cpp \
    ratemonitor.cpp \
    stringutil.cpp \
    threadutil.cpp \
    timespan.cpp \
    userutil.cpp

HEADERS += \
    addresshelper.h \
    cryptoutil.h \
    datetimeutil.h \
    geo/earthgeo.h \
    fileutil.h \
    geo/geocircle.h \
    geo/geocoordinate.h \
    geo/geotypes.h \
    jsonhelper.h \
    kanoopcommon.h \
    klog.h \
    lockingqueue.h \
    mutexevent.h \
    pathutil.h \
    ratemonitor.h \
    stringutil.h \
    threadutil.h \
    timeconstants.h \
    timespan.h \
    userutil.h

header_files.files = $$HEADERS
header_files.path = $$OUTPUT_PREFIX/usr/include/Kanoop

INSTALLS += target
INSTALLS += header_files

QMAKE_POST_LINK=$(MAKE) install
