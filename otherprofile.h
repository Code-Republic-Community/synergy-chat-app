#ifndef OTHERPROFILE_H
#define OTHERPROFILE_H

#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>
#include "v_chat_widget.h"

class OtherProfile : public QWidget
{
    Q_OBJECT
public:
    explicit OtherProfile(QWidget *parent = nullptr);
    void setLanguage();

signals:
    void goBackSignal();
    void logOutSiganl();
    void gotoSettingsSignal();

private:
    QLabel *profilePhoto;
    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *nicknameLabel;
    QPushButton *goBackButton;

private:
    void init();
    void setup();
    void styling();
    void connections();
};

#endif // OTHERPROFILE_H
