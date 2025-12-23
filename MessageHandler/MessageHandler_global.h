#ifndef MESSAGEHANDLER_GLOBAL_H
#define MESSAGEHANDLER_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#if defined(MESSAGEHANDLER_LIBRARY)
#define MESSAGEHANDLER_EXPORT Q_DECL_EXPORT
#else
#define MESSAGEHANDLER_EXPORT Q_DECL_IMPORT
#endif

#endif // MESSAGEHANDLER_GLOBAL_H
