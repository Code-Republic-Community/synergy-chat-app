#ifndef MYPROFILE_H
#define MYPROFILE_H

#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QIcon>
#include <QRegularExpression>

#include "httpclient.h"

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
    void settings_changed_successfully();
private slots:
    void handleProfileUpdate(QByteArray responseData);
    void handleProfileEditing();
public slots:
    void handleIdReceiving();

private:
    HttpClient *client_donwnload_profile_data;
    QString userId;

private:
    QHBoxLayout *topLayout;
    QWidget *topLayoutContainer;
    QHBoxLayout *bottomLayout;
    QWidget *bottomLayoutContainer;


    QLabel *profilePhoto;
    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *nicknameLabel;
    QLabel *emailLabel;
    QLabel *ageLabel;


    QLabel *nameSmallLabel;
    QLabel *surnameSmallLabel;
    QLabel *nicknameSmallLabel;
    QLabel *emailSmallLabel;
    QLabel *ageSmallLabel;

    QLineEdit *nameEdit;
    QLineEdit *surnameEdit;
    QLineEdit *nicknameEdit;

    QPushButton *goBackButton;
    QPushButton *gotoSettings;
    QPushButton *logOut;
    QPushButton *editProfile;
    QPushButton *changePhotoButton;
    QIcon *editIcon;
    bool is_photo_edited;
    QMap<int, QString> *oldDataMap;
    QMap<int, QString> *newDataMap;

    bool isEditing = false;

private:
    QRegularExpression nameRegex;
    QRegularExpression surnameRegex;
    QRegularExpression nicknameRegex;

private:
    void init();
    void setup();
    void styling();
    void connections();
    void toggleEditMode(bool enable);
    void saveChanges();
    void createLabelPair(const QString &smallText, QLabel *&smallLabel, QLabel *&mainLabel);
    void setupLabelPosition(QLabel *smallLabel, QLabel *mainLabel, int y);
};

#endif // MYPROFILE_H
