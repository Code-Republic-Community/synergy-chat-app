#ifndef GLOBALS_H
#define GLOBALS_H

#include <QPair>
#include <QString>
#include <QPixmap>

class Globals
{
private:
    Globals();
public:
    static Globals &getInstance();
    const QString &getUserId();
    void setUserID(const QString &id);
    QPixmap decodeBase64ToPixmap(const QString &base64String);
    QString encodeImageToBase64(const QPixmap &pixmap);
    QString getSERVER_LINK() const;
    void setSERVER_LINK(const QString &newSERVER_LINK);

    QString getWEB_SOCKET_LINK() const;
    void setWEB_SOCKET_LINK(const QString &newWEB_SOCKET_LINK);

private:
    QString USER_ID;
    QString SERVER_LINK;
    QString WEB_SOCKET_LINK;
private:
    Globals(const Globals &) = delete;
    Globals &operator=(const Globals &) = delete;
    Globals(Globals &&) = delete;
    Globals &operator=(Globals &&) = delete;
};

#endif // GLOBALS_H
