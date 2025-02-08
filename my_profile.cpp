#include "my_profile.h"
#include <QDebug>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QStringList>
#include <QUrl>
#include "globals.h"
#include "httpclient.h"

MyProfile::MyProfile(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 700);
    this->setFixedSize(400, 700);
    init();
    connections();
    setup();
    setLanguage();
    styling();
}

void MyProfile::handleProfileUpdate(QByteArray responseData)
{
    qDebug() << "________________MYProfile request_________________";

    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (jsonResponse.isNull() || !jsonResponse.isObject()) {
        qDebug() << "Invalid JSON response.";
        return;
    }

    QJsonObject jsonObject = jsonResponse.object();

    QString surname = jsonObject.value("surname").toString();
    QString nickname = jsonObject.value("nickname").toString();
    QString email = jsonObject.value("email").toString();
    QString name = jsonObject.value("name").toString();
    QString date_of_birth = jsonObject.value("date_of_birth").toString();

    if (nameLabel) {
        nameLabel->setText("Name: " + name);
    }
    if (surnameLabel) {
        surnameLabel->setText(tr("Surname: ") + surname);
    }
    if (nicknameLabel) {
        nicknameLabel->setText(tr("Nickname: ") + nickname);
    }
    if (emailLabel) {
        emailLabel->setText(tr("Email: ") + email);
    }
    if (ageLabel) {
        ageLabel->setText("Date of Birth: " + date_of_birth);
    }
    oldDataMap = newDataMap;
}

void MyProfile::handleIdReceiving()
{
    connect(client_donwnload_profile_data,
            &HttpClient::responseReceived,
            this,
            &MyProfile::handleProfileUpdate);
    QString link("https://synergy-iauu.onrender.com/profile_info/");
    QUrl accountinfo(link + Globals::getInstance().getUserId());
    client_donwnload_profile_data->getRequest(accountinfo);
}
void MyProfile::setLanguage()
{
    if (changePhotoButton) {
        changePhotoButton->setText(tr("Change"));
    }

    if (goBackButton) {
        goBackButton->setText(tr("Back"));
    }

    if (gotoSettings) {
        gotoSettings->setText(tr("Settings"));
    }

    if (logOut) {
        logOut->setText(tr("Log Out"));
    }

    if (editProfile) {
        editProfile->setText(tr("Edit"));
    }
}

void MyProfile::init()
{
    client_donwnload_profile_data = new HttpClient();

    profilePhoto = new QLabel(this);
    nameLabel = new QLabel(this);
    surnameLabel = new QLabel(this);
    nicknameLabel = new QLabel(this);
    emailLabel = new QLabel(this);
    ageLabel = new QLabel(this);

    nameEdit = new QLineEdit(this);
    surnameEdit = new QLineEdit(this);
    nicknameEdit = new QLineEdit(this);

    nameEdit->hide();
    surnameEdit->hide();
    nicknameEdit->hide();

    goBackButton = new QPushButton(this);
    gotoSettings = new QPushButton(this);
    logOut = new QPushButton(this);
    editProfile = new QPushButton(this);

    changePhotoButton = new QPushButton(this);
    changePhotoButton->hide();

    oldDataMap = new QMap<int, QString>();
    newDataMap = new QMap<int, QString>();
}

void MyProfile::setup()
{
    QString defaultPhotoPath = ":/pngs/panda.jpg";

    nameLabel->setText("John");
    surnameLabel->setText("Doe");
    nicknameLabel->setText("@johndoe");
    emailLabel->setText("johndoe2004@gmail.com");
    ageLabel->setText("02.02.2004");

    oldDataMap->insert(0, nameLabel->text());
    oldDataMap->insert(1, surnameLabel->text());
    oldDataMap->insert(2, nicknameLabel->text());

    goBackButton->setGeometry(10, 10, 80, 40);
    profilePhoto->setGeometry(150, 10, 100, 100);
    editProfile->setGeometry(310, 10, 80, 40);

    nameLabel->setGeometry(20, 130, 360, 30);
    surnameLabel->setGeometry(20, 170, 360, 30);
    nicknameLabel->setGeometry(20, 210, 360, 30);
    emailLabel->setGeometry(20, 250, 360, 30);
    ageLabel->setGeometry(20, 290, 360, 30);

    nameEdit->setGeometry(nameLabel->geometry());
    surnameEdit->setGeometry(surnameLabel->geometry());
    nicknameEdit->setGeometry(nicknameLabel->geometry());

    gotoSettings->setGeometry(20, 640, 170, 40);
    logOut->setGeometry(210, 640, 170, 40);
    changePhotoButton->setGeometry(260, 70, 120, 40);

    QPixmap profilePic(VChatWidget::cut_photo(defaultPhotoPath, 100));
    if (!profilePic.isNull()) {
        profilePhoto->setPixmap(
            profilePic.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "Failed to load default profile photo.";
    }
}

void MyProfile::styling()
{
    QString labelStyle = R"(
        QLabel {
            color: #FFFFFF;
            font-size: 12px;
            border: 3px solid #420242;
            border-radius: 5px;
            padding: 3px;
        }
    )";

    nameLabel->setStyleSheet(labelStyle);
    surnameLabel->setStyleSheet(labelStyle);
    nicknameLabel->setStyleSheet(labelStyle);
    emailLabel->setStyleSheet(labelStyle);
    ageLabel->setStyleSheet(labelStyle);

    QString lineEditStyle = R"(
        QLineEdit {
            color: #FFFFFF;
            font-size: 12px;
            border: 3px solid #420242;
            border-radius: 5px;
            padding: 3px;
            background-color: #2A002A;
        }
    )";

    nameEdit->setStyleSheet(lineEditStyle);
    surnameEdit->setStyleSheet(lineEditStyle);
    nicknameEdit->setStyleSheet(lineEditStyle);

    QString buttonStyle = R"(
        QPushButton {
            border: 3px solid #420242;
            border-radius: 5px;
            padding: 8px 15px;
            background-color: #420242;
            color: white;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5A005A;
        }
        QPushButton:pressed {
            background-color: #2A002A;
        }
    )";

    goBackButton->setStyleSheet(buttonStyle);
    gotoSettings->setStyleSheet(buttonStyle);
    logOut->setStyleSheet(buttonStyle);
    editProfile->setStyleSheet(buttonStyle);
    changePhotoButton->setStyleSheet(buttonStyle);

    profilePhoto->setAlignment(Qt::AlignCenter);
}

void MyProfile::connections()
{
    connect(editProfile, &QPushButton::clicked, this, [this]() {
        if (isEditing) {
            saveChanges();
        } else {
            toggleEditMode(true);
        }
    });

    connect(goBackButton, &QPushButton::clicked, this, [this]() {
        if (isEditing) {
            toggleEditMode(false);
        } else {
            qDebug() << "go back siganl";
            emit goBackSignal();
        }
    });

    connect(changePhotoButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this,
                                                        "Select Profile Photo",
                                                        QDir::homePath(),
                                                        "Images (*.png *.xpm *.jpg *.jpeg *.bmp)");

        if (!filePath.isEmpty()) {
            QPixmap newProfilePic(filePath);
            if (!newProfilePic.isNull()) {
                profilePhoto->setPixmap(VChatWidget::cut_photo(filePath, 100));
                qDebug() << "Profile photo changed to:" << filePath;
            } else {
                qDebug() << "Failed to load selected image.";
            }
        } else {
            qDebug() << "No file selected.";
        }
    });

    connect(logOut, &QPushButton::clicked, this, [this]() {
        qDebug() << "log out siganl";
        emit logOutSiganl();
    });

    connect(gotoSettings, &QPushButton::clicked, this, [this]() {
        qDebug() << "go to settings siganl";
        emit gotoSettingsSignal();
    });
}

void MyProfile::toggleEditMode(bool enable)
{
    isEditing = enable;

    changePhotoButton->setVisible(enable);
    nameEdit->setVisible(enable);
    surnameEdit->setVisible(enable);
    nicknameEdit->setVisible(enable);

    nameLabel->setVisible(!enable);
    surnameLabel->setVisible(!enable);
    nicknameLabel->setVisible(!enable);

    if (enable) {
        nameEdit->setText(nameLabel->text());
        surnameEdit->setText(surnameLabel->text());
        nicknameEdit->setText(nicknameLabel->text());

        newDataMap->insert(0, nameLabel->text());
        newDataMap->insert(1, surnameLabel->text());
        newDataMap->insert(2, nicknameLabel->text());

        editProfile->setText(tr("Save"));
        goBackButton->setText(tr("Cancel"));
    } else {
        nameLabel->setText(oldDataMap->value(0));
        surnameLabel->setText(oldDataMap->value(1));
        nicknameLabel->setText(oldDataMap->value(2));

        editProfile->setText(tr("Edit"));
        goBackButton->setText(tr("Back"));
    }
}

void MyProfile::saveChanges()
{
    oldDataMap->insert(0, nameEdit->text());
    oldDataMap->insert(1, surnameEdit->text());
    oldDataMap->insert(2, nicknameEdit->text());

    nameLabel->setText(nameEdit->text());
    surnameLabel->setText(surnameEdit->text());
    nicknameLabel->setText(nicknameEdit->text());

    newDataMap->insert(0, nameEdit->text());
    newDataMap->insert(1, surnameEdit->text());
    newDataMap->insert(2, nicknameEdit->text());

    toggleEditMode(false);
}
