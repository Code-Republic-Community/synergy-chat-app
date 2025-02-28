#include "globals.h"

#include <QFile>
#include <QDebug>
#include <QBuffer>

Globals &Globals::getInstance()
{
    static Globals instance;
    return instance;
}

const QString &Globals::getUserId()
{
    return USER_ID;
}

void Globals::setUserID(const QString &id)
{
    USER_ID = id;
}


Globals::Globals()
{
    USER_ID = "";
}


QString Globals::encodeImageToBase64(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        qDebug() << "Error: Invalid QPixmap!";
        return "";
    }

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    if (!pixmap.save(&buffer, "JPEG")) {
        qDebug() << "Error saving QPixmap to buffer!";
        return "";
    }

    return QString(byteArray.toBase64());
}

QPixmap Globals::decodeBase64ToPixmap(const QString &base64String)
{
    QByteArray imageData = QByteArray::fromBase64(base64String.toUtf8());
    QPixmap pixmap;

    if (!pixmap.loadFromData(imageData, "JPEG")) {
        if (!pixmap.loadFromData(imageData, "PNG")) {
            qDebug() << "Failed to decode image from Base64!";
            return QPixmap(":/pngs/panda.jpg");
        }
    }
    return pixmap;
}
