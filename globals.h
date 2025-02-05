#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>
#include <QPair>

class Globals
{
private:
    Globals();
public:
    static Globals& getInstance();
    const QString& getUserId();
    void setUserID(const QString& id);
private:
    QPair<QString, QString> comboBoxStyle;
    QPair<QString, QString> buttonStyle;
    QPair<QString, QString> formLabelStyle;
    QPair<QString, QString> labelStyle;
    QPair<QString, QString> checkBoxStyle;
    QString USER_ID;
private:
    Globals(const Globals&) = delete;
    Globals& operator=(const Globals&) = delete;
    Globals(Globals&&) = delete;
    Globals& operator=(Globals&&) = delete;
};

#endif // GLOBALS_H
