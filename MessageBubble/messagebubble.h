#ifndef MESSAGEBUBBLE_H
#define MESSAGEBUBBLE_H

#include "MessageBubble_global.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFrame>
#include <QDateTime>
#include <QPainter>
#include <QPainterPath>

class MESSAGEBUBBLE_EXPORT MessageBubble : public QWidget
{
    Q_OBJECT

public:
    explicit MessageBubble(const QString &sender, const QString &text,
                           const QString &avatarPath, bool isMyMessage,
                           QWidget *parent = nullptr);

private:
    void setupUi(const QString &sender, const QString &text,
                 const QString &avatarPath, bool isMyMessage);
    QPixmap getCircularPixmap(const QPixmap &src, int size);
};

#endif // MESSAGEBUBBLE_H
