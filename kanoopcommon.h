#ifndef KANOOPCOMMON_H
#define KANOOPCOMMON_H
#include <QtCore/qglobal.h>

#if defined(KANOOP_LIBRARY)
#  define KANOOP_EXPORT Q_DECL_EXPORT
#else
#  define KANOOP_EXPORT Q_DECL_IMPORT
#endif


#endif // KANOOPCOMMON_H
