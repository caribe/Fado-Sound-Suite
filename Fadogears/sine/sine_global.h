#ifndef SINE_GLOBAL_H
#define SINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SINE_LIBRARY)
#  define SINESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SINESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SINE_GLOBAL_H
