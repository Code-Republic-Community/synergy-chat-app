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
#include "v_chat_widget.h"

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

    profilePhoto->setPixmap(VChatWidget::cut_photo(QPixmap(), 100));

    if (nameLabel) {
        nameLabel->setText(name);
    }
    if (surnameLabel) {
        surnameLabel->setText(surname);
    }
    if (nicknameLabel) {
        nicknameLabel->setText(nickname);
    }
    if (emailLabel) {
        emailLabel->setText(email);
    }
    if (ageLabel) {
        ageLabel->setText(date_of_birth);
    }
    oldDataMap = newDataMap;
}

void MyProfile::handleProfileEditing() //petqa grvi
{
//     QJsonObject jsonData;
//     jsonData["name"] = newDataMap->value(0);
//     jsonData["surname"] = newDataMap->value(1);
//     jsonData["nickname"] = newDataMap->value(2);
//     QUrl putRequestLink("https://synergy-iauu.onrender.com/edit_profile/" + Globals::getInstance().getUserId() + "/");
//     connect(client_donwnload_profile_data, &HttpClient::responseReceived, this, [=](QByteArray responseData){
//         QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//         QJsonObject jsonObject = jsonResponse.object();
//         if(jsonObject.contains("message") && jsonObject["message"].toString() == "Profile updated successfully")
//         {
//             oldDataMap->insert(2, newDataMap->take(2));
//         }
//         else
//         {
//             newDataMap->insert(2, oldDataMap->take(2));
//             nicknameLabel->setText(newDataMap->take(2));
//             nicknameEdit->setText(nicknameLabel->text());
//         }
//     });
//     client_donwnload_profile_data->putRequest(putRequestLink, jsonData); //hat hat put anem vor nickname y zbaxvac lini mnacacy ashxati ...
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

    if (nameSmallLabel) nameSmallLabel->setText(tr("Name"));
    if (surnameSmallLabel) surnameSmallLabel->setText(tr("Surname"));
    if (nicknameSmallLabel) nicknameSmallLabel->setText(tr("Nickname"));
    if (emailSmallLabel) emailSmallLabel->setText(tr("Email"));
    if (ageSmallLabel) ageSmallLabel->setText(tr("Date of Birth"));
}

void MyProfile::init()
{
    client_donwnload_profile_data = new HttpClient();
    is_photo_edited = false;
    topLayoutContainer = new QWidget(this);
    topLayout = new QHBoxLayout;

    bottomLayoutContainer = new QWidget(this);
    bottomLayout = new QHBoxLayout;

    profilePhoto = new QLabel(this);

    createLabelPair(tr("Name"), nameSmallLabel, nameLabel);
    createLabelPair(tr("Surname"), surnameSmallLabel, surnameLabel);
    createLabelPair(tr("Nickname"), nicknameSmallLabel, nicknameLabel);
    createLabelPair(tr("Email"), emailSmallLabel, emailLabel);
    createLabelPair(tr("Date of Birth"), ageSmallLabel, ageLabel);

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
    editIcon = new QIcon(":/pngs/edit.ico");
    oldDataMap = new QMap<int, QString>();
    newDataMap = new QMap<int, QString>();
}

void MyProfile::setup()
{
    QPixmap picture(":/pngs/panda.jpg");
    topLayoutContainer ->setLayout(topLayout);
    topLayoutContainer->setGeometry(0, 0, 400, 60);

    bottomLayoutContainer->setLayout(bottomLayout);
    bottomLayoutContainer->setGeometry(0, 640, 400, 60);

    nameLabel->setText("John");
    surnameLabel->setText("Doe");
    nicknameLabel->setText("@johndoe");
    emailLabel->setText("johndoe2@gmail.com");
    ageLabel->setText("02.02.2004");

    oldDataMap->insert(0, nameLabel->text());
    oldDataMap->insert(1, surnameLabel->text());
    oldDataMap->insert(2, nicknameLabel->text());

    topLayout ->addWidget(goBackButton);

    topLayout ->addWidget(editProfile);

    profilePhoto->setGeometry(150, 60, 100, 100);

    topLayout->setAlignment(editProfile, Qt::AlignRight);
    topLayout->setAlignment(goBackButton, Qt::AlignLeft);



    setupLabelPosition(nameSmallLabel, nameLabel, 180);
    setupLabelPosition(surnameSmallLabel, surnameLabel, 240);
    setupLabelPosition(nicknameSmallLabel, nicknameLabel, 300);
    setupLabelPosition(emailSmallLabel, emailLabel, 360);
    setupLabelPosition(ageSmallLabel, ageLabel, 420);

    nameEdit->setGeometry(nameLabel->geometry());
    surnameEdit->setGeometry(surnameLabel->geometry());
    nicknameEdit->setGeometry(nicknameLabel->geometry());

    bottomLayout->addWidget(gotoSettings);
    bottomLayout->addWidget(logOut);

    changePhotoButton->setIcon(*editIcon);
    changePhotoButton->setGeometry(230, 120, 20, 20);
    changePhotoButton->setIconSize(changePhotoButton->size());

    oldDataMap->insert(0, nameLabel->text());
    oldDataMap->insert(1, surnameLabel->text());
    oldDataMap->insert(2, nicknameLabel->text());
}

void MyProfile::createLabelPair(const QString &smallText, QLabel *&smallLabel, QLabel *&mainLabel)
{
    smallLabel = new QLabel(smallText, this);
    smallLabel->setStyleSheet("font-size: 10px; color: white;");

    mainLabel = new QLabel(this);
    mainLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
}

void MyProfile::setupLabelPosition(QLabel *smallLabel, QLabel *mainLabel, int y)
{

    smallLabel->setGeometry(10, y - 17, 380, 15);
    mainLabel->setGeometry(10, y, 380, 30);
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
    goBackButton->setFocusPolicy(Qt::NoFocus);
    gotoSettings->setFocusPolicy(Qt::NoFocus);
    logOut->setFocusPolicy(Qt::NoFocus);
    editProfile->setFocusPolicy(Qt::NoFocus);
    profilePhoto->setAlignment(Qt::AlignCenter);


}

void MyProfile::connections()
{

    connect(this, &MyProfile::settings_changed_successfully, this, &MyProfile::handleProfileEditing);

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
                                                        "Images (*.png *.jpg *.jpeg)");

        if (!filePath.isEmpty()) {
            QPixmap newProfilePic(filePath);
            profilePhoto->setPixmap(VChatWidget::cut_photo(newProfilePic, 100));
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
    }
    else
    {
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

    nameLabel->setText(nameEdit->text());
    surnameLabel->setText(surnameEdit->text());
    nicknameLabel->setText(nicknameEdit->text());

    newDataMap->insert(0, nameEdit->text());
    newDataMap->insert(1, surnameEdit->text());
    newDataMap->insert(2, nicknameEdit->text());
    emit settings_changed_successfully();
    toggleEditMode(false);
}
