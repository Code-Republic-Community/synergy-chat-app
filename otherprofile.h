#ifndef OTHERPROFILE_H
#define OTHERPROFILE_H

#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>
#include "httpclient.h"

class OtherProfile : public QWidget
{
    Q_OBJECT
public:
    explicit OtherProfile(QWidget *parent = nullptr);
    void setLanguage();
public slots:
    void handleDataFromChat(QString nick);

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
    HttpClient *client_other_profile;

private:
    void init();
    void setup();
    void styling();
    void connections();
};

#endif // OTHERPROFILE_H
