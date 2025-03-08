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
    // SERVER_LINK = "https://synergy-iauu.onrender.com/";
    // WEB_SOCKET_LINK = "wss://synergy-iauu.onrender.com/ws/";
    SERVER_LINK = "http://127.0.0.1:8000/";
    WEB_SOCKET_LINK = "ws://127.0.0.1:8000/ws/";
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

QString Globals::getSERVER_LINK() const
{
    return SERVER_LINK;
}

void Globals::setSERVER_LINK(const QString &newSERVER_LINK)
{
    SERVER_LINK = newSERVER_LINK;
}

QString Globals::getWEB_SOCKET_LINK() const
{
    return WEB_SOCKET_LINK;
}

void Globals::setWEB_SOCKET_LINK(const QString &newWEB_SOCKET_LINK)
{
    WEB_SOCKET_LINK = newWEB_SOCKET_LINK;
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
