#include "messagebubble.h"
#include "qpainter.h"

MessageBubble::MessageBubble(const QString &sender, const QString &text,
                             const QString &avatarPath, bool isMyMessage,
                             QWidget *parent)
    : QWidget(parent)
{
    setupUi(sender, text, avatarPath, isMyMessage);
}

void MessageBubble::setupUi(const QString &sender, const QString &text,
                            const QString &avatarPath, bool isMyMessage)
{
    // --- 1. Globales Layout (Links/Rechts Ausrichtung) ---
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);

    // --- 2. Der Bubble Container (Der farbige Kasten) ---
    QFrame *bubbleFrame = new QFrame(this);
    bubbleFrame->setObjectName("bubbleFrame"); // Für CSS
    // Vertikales Layout IN der Blase: Header oben, Text mitte, Zeit unten
    QVBoxLayout *bubbleContentLayout = new QVBoxLayout(bubbleFrame);
    bubbleContentLayout->setContentsMargins(10, 10, 10, 5); // Innenabstand
    bubbleContentLayout->setSpacing(5);

    // --- 3. Der Header (Icon + Name) ---
    QWidget *headerWidget = new QWidget(bubbleFrame);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(8); // Abstand Icon <-> Name

    // Avatar vorbereiten
    QLabel *iconLabel = new QLabel(headerWidget);
    QPixmap avatarPix(avatarPath);
    if(avatarPix.isNull()) {
        // Fallback, falls kein Bild da ist (z.B. grauer Kreis)
        avatarPix = QPixmap(32, 32);
        avatarPix.fill(Qt::gray);
    }
    iconLabel->setPixmap(getCircularPixmap(avatarPix, 32)); // 32px Größe
    iconLabel->setFixedSize(32, 32);

    // Name Label
    QLabel *nameLabel = new QLabel(sender, headerWidget);
    nameLabel->setObjectName("nameLabel"); // Für CSS (Fett gedruckt)

    // Layout-Logik für den Header:
    if (isMyMessage) {
        // MEINE NACHRICHT: Name Links, Icon Rechts (oder Spacer dazwischen)
        headerLayout->addStretch();      // Schiebt alles nach rechts
        headerLayout->addWidget(nameLabel);
        headerLayout->addWidget(iconLabel);
    } else {
        // FREMDE NACHRICHT: Icon Links, Name Rechts
        headerLayout->addWidget(iconLabel);
        headerLayout->addWidget(nameLabel);
        headerLayout->addStretch();      // Schiebt Rest nach rechts (damit Name nicht zentriert ist)
    }

    // --- 4. Der Nachrichtentext ---
    QLabel *msgLabel = new QLabel(text, bubbleFrame);
    msgLabel->setWordWrap(true);
    msgLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgLabel->setObjectName("msgLabel");

    // --- 5. Der Zeitstempel ---
    QLabel *timeLabel = new QLabel(QTime::currentTime().toString("HH:mm"), bubbleFrame);
    timeLabel->setAlignment(Qt::AlignRight);
    timeLabel->setObjectName("timeLabel");

    // --- 6. Alles zusammenbauen ---
    bubbleContentLayout->addWidget(headerWidget); // Header Zeile
    bubbleContentLayout->addWidget(msgLabel);     // Text
    bubbleContentLayout->addWidget(timeLabel);    // Zeit

    // --- 7. Positionierung der Blase im Main Layout ---
    if(isMyMessage) {
        mainLayout->addStretch();
        mainLayout->addWidget(bubbleFrame);
    } else {
        mainLayout->addWidget(bubbleFrame);
        mainLayout->addStretch();
    }

    // --- 8. Styling (CSS) ---
    // Wir nutzen CSS Variablen-Tricks oder einfache String-Logik
    QString bgColor = isMyMessage ? "#DCF8C6" : "#FFFFFF"; // Grün vs Weiß
    QString nameColor = isMyMessage ? "#2E7D32" : "#D32F2F"; // Dunkelgrün vs Rot

    // Runde Ecken Logik
    QString radius = isMyMessage ? "border-top-right-radius: 0px;" : "border-top-left-radius: 0px;";

    bubbleFrame->setStyleSheet(QString(R"(
        QFrame#bubbleFrame {
            background-color: %1;
            border-radius: 15px;
            %2
        }
        QLabel#nameLabel {
            color: %3;
            font-weight: bold;
            font-size: 12px;
        }
        QLabel#msgLabel {
            color: #202020;
            font-size: 14px;
            padding-top: 5px;
        }
        QLabel#timeLabel {
            color: #808080;
            font-size: 10px;
        }
    )").arg(bgColor, radius, nameColor));

    // Begrenzung der Breite
    bubbleFrame->setMaximumWidth(1024);
    bubbleFrame->setMinimumWidth(150);
}

QPixmap MessageBubble::getCircularPixmap(const QPixmap &src, int size) {
    if (src.isNull()) return QPixmap();

    // Skalieren auf die Zielgröße (z.B. 32x32)
    QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // Zurechtschneiden auf Quadrat
    QPixmap squared = scaled.copy((scaled.width() - size)/2, (scaled.height() - size)/2, size, size);

    // Runde Maske erstellen
    QPixmap dest(size, size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, squared);

    return dest;
}

