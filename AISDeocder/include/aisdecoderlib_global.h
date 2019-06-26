#ifndef AISDECODERLIB_GLOBAL_H
#define AISDECODERLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AISDECODERLIB_LIBRARY)
#  define AISDECODERLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define AISDECODERLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // AISDECODERLIB_GLOBAL_H
