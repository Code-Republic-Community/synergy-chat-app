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
    void handleDataFromChat(QString nickname, QString name, QString surname , QPixmap photo);

signals:
    void goBackSignal();
    void logOutSiganl();
    void gotoSettingsSignal();

private:
    QLabel *profilePhoto;
    QPushButton *goBackButton;
    HttpClient *client_other_profile;

    QLabel *nameSmallLabel;
    QLabel *surnameSmallLabel;
    QLabel *nicknameSmallLabel;

    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *nicknameLabel;



private:
    void createLabelPair(const QString &smallText, QLabel *&smallLabel, QLabel *&mainLabel);
    void setupLabelPosition(QLabel *smallLabel, QLabel *mainLabel, int y);


private:
    void init();
    void setup();
    void styling();
    void connections();

};

#endif // OTHERPROFILE_H
