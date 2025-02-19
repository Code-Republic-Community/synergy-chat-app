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
private:
    QPair<QString, QString> comboBoxStyle;
    QPair<QString, QString> buttonStyle;
    QPair<QString, QString> formLabelStyle;
    QPair<QString, QString> labelStyle;
    QPair<QString, QString> checkBoxStyle;
    QString USER_ID;

private:
    Globals(const Globals &) = delete;
    Globals &operator=(const Globals &) = delete;
    Globals(Globals &&) = delete;
    Globals &operator=(Globals &&) = delete;
};

#endif // GLOBALS_H
