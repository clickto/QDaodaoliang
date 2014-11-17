#ifndef QCOMMONKIT_GLOBAL_H
#define QCOMMONKIT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QCOMMONKIT_LIBRARY)
#  define QCOMMONKITSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QCOMMONKITSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QCOMMONKIT_GLOBAL_H
