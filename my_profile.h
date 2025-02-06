#ifndef MYPROFILE_H
#define MYPROFILE_H

#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include "httpclient.h"
#include "v_chat_widget.h"

class MyProfile : public QWidget
{
    Q_OBJECT
public:
    explicit MyProfile(QWidget *parent = nullptr);
    void setLanguage();
signals:
    void goBackSignal();
    void logOutSiganl();
    void gotoSettingsSignal();
private slots:
    void handleProfileUpdate(QByteArray responseData);
public slots:
    void handleIdReceiving();

private:
    HttpClient *client_donwnload_profile_data;
    QString userId;

private:
    QLabel *profilePhoto;
    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *nicknameLabel;
    QLabel *emailLabel;
    QLabel *ageLabel;

    QLineEdit *nameEdit;
    QLineEdit *surnameEdit;
    QLineEdit *nicknameEdit;

    QPushButton *goBackButton;
    QPushButton *gotoSettings;
    QPushButton *logOut;
    QPushButton *editProfile;
    QPushButton *changePhotoButton;

    QMap<int, QString> *oldDataMap;
    QMap<int, QString> *newDataMap;

    bool isEditing = false;

private:
    void init();
    void setup();
    void styling();
    void connections();
    void toggleEditMode(bool enable);
    void saveChanges();
};

#endif // MYPROFILE_H
