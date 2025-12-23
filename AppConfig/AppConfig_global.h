#ifndef APPCONFIG_GLOBAL_H
#define APPCONFIG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(APPCONFIG_LIBRARY)
#define APPCONFIG_EXPORT Q_DECL_EXPORT
#else
#define APPCONFIG_EXPORT Q_DECL_IMPORT
#endif

#endif // APPCONFIG_GLOBAL_H
