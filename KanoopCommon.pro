QT       += network
QT       -= gui

TEMPLATE = lib
VERSION=1.0.21

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

INCLUDEPATH += include

target.path = $$OUTPUT_PREFIX/usr/lib

SOURCES += \
    src/Kanoop/addresshelper.cpp \
    src/Kanoop/commonexception.cpp \
    src/Kanoop/cryptoutil.cpp \
    src/Kanoop/datetimeutil.cpp \
    src/Kanoop/fileutil.cpp \
    src/Kanoop/geo/earthgeo.cpp \
    src/Kanoop/geo/geocircle.cpp \
    src/Kanoop/geo/geocoordinate.cpp \
    src/Kanoop/geo/geotypes.cpp \
    src/Kanoop/geometry/angle.cpp \
    src/Kanoop/geometry/circle.cpp \
    src/Kanoop/geometry/flatgeo.cpp \
    src/Kanoop/geometry/geo.cpp \
    src/Kanoop/geometry/line.cpp \
    src/Kanoop/geometry/pathrouter.cpp \
    src/Kanoop/geometry/point.cpp \
    src/Kanoop/geometry/polygon.cpp \
    src/Kanoop/geometry/rectangle.cpp \
    src/Kanoop/geometry/spanningtree.cpp \
    src/Kanoop/geometry/treepathvertice.cpp \
    src/Kanoop/geometry/triangle.cpp \
    src/Kanoop/jsonhelper.cpp \
    src/Kanoop/klog.cpp \
    src/Kanoop/mutexevent.cpp \
    src/Kanoop/pathutil.cpp \
    src/Kanoop/ratemonitor.cpp \
    src/Kanoop/stringutil.cpp \
    src/Kanoop/threadutil.cpp \
    src/Kanoop/timespan.cpp \
    src/Kanoop/userutil.cpp

HEADERS += \
    include/Kanoop/addresshelper.h \
    include/Kanoop/commonexception.h \
    include/Kanoop/cryptoutil.h \
    include/Kanoop/datetimeutil.h \
    include/Kanoop/fileutil.h \
    include/Kanoop/geo/earthgeo.h \
    include/Kanoop/geo/geocircle.h \
    include/Kanoop/geo/geocoordinate.h \
    include/Kanoop/geo/geotypes.h \
    include/Kanoop/geometry/angle.h \
    include/Kanoop/geometry/circle.h \
    include/Kanoop/geometry/flatgeo.h \
    include/Kanoop/geometry/geo.h \
    include/Kanoop/geometry/line.h \
    include/Kanoop/geometry/pathrouter.h \
    include/Kanoop/geometry/point.h \
    include/Kanoop/geometry/polygon.h \
    include/Kanoop/geometry/rectangle.h \
    include/Kanoop/geometry/spanningtree.h \
    include/Kanoop/geometry/treepathvertice.h \
    include/Kanoop/geometry/triangle.h \
    include/Kanoop/jsonhelper.h \
    include/Kanoop/kanoopcommon.h \
    include/Kanoop/klog.h \
    include/Kanoop/lockingqueue.h \
    include/Kanoop/mutexevent.h \
    include/Kanoop/pathutil.h \
    include/Kanoop/ratemonitor.h \
    include/Kanoop/stringutil.h \
    include/Kanoop/threadutil.h \
    include/Kanoop/timeconstants.h \
    include/Kanoop/timespan.h \
    include/Kanoop/userutil.h

header_files.files = $$HEADERS
header_files.path = $$OUTPUT_PREFIX/usr/include/Kanoop

INSTALLS += target
INSTALLS += header_files

QMAKE_POST_LINK=$(MAKE) install
