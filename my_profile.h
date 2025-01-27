#ifndef MYPROFILE_H
#define MYPROFILE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMap>
#include <QString>

class MyProfile : public QWidget
{
    Q_OBJECT
public:
    explicit MyProfile(QWidget *parent = nullptr);

signals:
    void goBackSignal();
    void logOutSiganl();
    void gotoSettingsSignal();

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
