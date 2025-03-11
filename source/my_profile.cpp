#include <QDebug>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QStringList>
#include <QUrl>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QRegularExpressionMatch>
#include <QStyle>

#include "my_profile.h"
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
    disconnect(client_donwnload_profile_data, &HttpClient::responseReceived, this, &MyProfile::handleProfileUpdate);
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
    encoded_photo = jsonObject.value("profile_photo").toString();
    QPixmap photo = Globals::getInstance().decodeBase64ToPixmap(encoded_photo);
    profilePhoto->setPixmap(VChatWidget::cut_photo(photo, 100));


    oldDataMap->insert(3, encoded_photo);

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

void MyProfile::handleProfileEditing(QByteArray responseData)
{
    disconnect(client_donwnload_profile_data, &HttpClient::responseReceived, this, &MyProfile::handleProfileEditing);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();
    if(jsonObject.contains("message") && jsonObject["message"].toString() == "Profile updated successfully")
    {
        qDebug()<< "Profile edited successfully";
        toggleEditMode(false);
        overlay->hideOverlay();
    }
    else
    {
        toggleEditMode(false);
        nicknameLabel->setText(newDataMap->value(2));
        overlay->hideOverlay();
    }
    handleIdReceiving();
}

void MyProfile::handleIdReceiving()
{
    disconnect(client_donwnload_profile_data,&HttpClient::responseReceived,this, &MyProfile::handleProfileUpdate);
    connect(client_donwnload_profile_data,&HttpClient::responseReceived,this, &MyProfile::handleProfileUpdate);
    QString link(Globals::getInstance().getSERVER_LINK() + "profile_info/");
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
    overlay = new LoadingOverlay(this);
    overlay->hideOverlay();
    nameRegex = QRegularExpression("^[a-zA-Z]{2,}$");
    surnameRegex = QRegularExpression("^[a-zA-Z]{1,}$");
    nicknameRegex = QRegularExpression("^[a-zA-Z][a-zA-Z0-9_]{2,}$");

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
    nameLabel->setObjectName("nameLabel");
    surnameLabel->setObjectName("surnameLabel");
    nicknameLabel->setObjectName("nicknameLabel");
    emailLabel->setObjectName("emailLabel");
    ageLabel->setObjectName("ageLabel");
    nameSmallLabel->setObjectName("nameSmallLabel");
    surnameSmallLabel->setObjectName("surnameSmallLabel");
    nicknameSmallLabel->setObjectName("nicknameSmallLabel");
    emailSmallLabel->setObjectName("emailSmallLabel");
    ageSmallLabel->setObjectName("ageSmallLabel");

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
    goBackButton->setMinimumSize(80, 40);
    gotoSettings->setMinimumSize(80, 40);
    logOut->setMinimumSize(80, 40);
    editProfile->setMinimumSize(80, 40);

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
    topLayoutContainer->setGeometry(0, 0, 400, 70);

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

    topLayout->setContentsMargins(20, 0, 20, 0);
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

    bottomLayout->setContentsMargins(20, 0, 20, 20);

    bottomLayout->addStretch();
    bottomLayout->addWidget(gotoSettings);
    bottomLayout->addItem(new QSpacerItem(20, 0));
    bottomLayout->addWidget(logOut);
    bottomLayout->addStretch();


    changePhotoButton->setIcon(*editIcon);
    changePhotoButton->setGeometry(230, 120, 20, 20);
    changePhotoButton->setIconSize(changePhotoButton->size());

    oldDataMap->insert(0, nameLabel->text());
    oldDataMap->insert(1, surnameLabel->text());
    oldDataMap->insert(2, nicknameLabel->text());
    oldDataMap->insert(3, Globals::getInstance().encodeImageToBase64(picture));
}

void MyProfile::createLabelPair(const QString &smallText, QLabel *&smallLabel, QLabel *&mainLabel)
{
    smallLabel = new QLabel(smallText, this);
    mainLabel = new QLabel(this);
}

void MyProfile::setupLabelPosition(QLabel *smallLabel, QLabel *mainLabel, int y)
{

    smallLabel->setGeometry(25, y - 20, 360, 15);
    mainLabel->setGeometry(20, y, 360, 30);
}

void MyProfile::styling()
{
    goBackButton->setFocusPolicy(Qt::NoFocus);
    gotoSettings->setFocusPolicy(Qt::NoFocus);
    logOut->setFocusPolicy(Qt::NoFocus);
    editProfile->setFocusPolicy(Qt::NoFocus);
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
                                                        "Images (*.png *.jpg *.jpeg)");
        if (!filePath.isEmpty()) {
            QPixmap newProfilePic(filePath);
            encoded_photo = Globals::getInstance().encodeImageToBase64(newProfilePic);
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

    connect(nameEdit, &QLineEdit::textEdited, this, [=]() {
        QRegularExpressionMatch match = nameRegex.match(nameEdit->text());
        if (match.hasMatch()) {
            editProfile->setEnabled(true);
            nameEdit->setProperty("error", false);
            nameEdit->style()->unpolish(nameEdit);
            nameEdit->style()->polish(nameEdit);
        }
        else {
            nameEdit->setProperty("error", true);
            nameEdit->style()->unpolish(nameEdit);
            nameEdit->style()->polish(nameEdit);
            editProfile->setDisabled(true);
        }
    });
    connect(surnameEdit, &QLineEdit::textEdited, this, [=]() {
        QRegularExpressionMatch match = surnameRegex.match(surnameEdit->text());
        if (match.hasMatch()) {
            surnameEdit->setProperty("error", false);
            surnameEdit->style()->unpolish(surnameEdit);
            surnameEdit->style()->polish(surnameEdit);
            editProfile->setEnabled(true);
        }
        else {
            surnameEdit->setProperty("error", true);
            surnameEdit->style()->unpolish(surnameEdit);
            surnameEdit->style()->polish(surnameEdit);
            editProfile->setDisabled(true);
        }
    });
    connect(nicknameEdit, &QLineEdit::textEdited, this, [=]() {
        if(nicknameEdit->text() == oldDataMap->value(2) ){
            editProfile->setEnabled(true);
            nicknameEdit->setProperty("error", false);
            nicknameEdit->style()->unpolish(nicknameEdit);
            nicknameEdit->style()->polish(nicknameEdit);
            return;
        }
        QRegularExpressionMatch match = nicknameRegex.match(nicknameEdit->text());
        if (match.hasMatch()) {
            editProfile->setEnabled(false);
            connect(client_donwnload_profile_data, &HttpClient::responseReceived, [=](QByteArray responseData){
                QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
                if(jsonDocument.isNull() || !jsonDocument.isObject())
                {
                    qDebug() << "Invalid Json Response";
                    editProfile->setDisabled(true);
                    nicknameEdit->setProperty("error", true);
                    nicknameEdit->style()->unpolish(nicknameEdit);
                    nicknameEdit->style()->polish(nicknameEdit);
                    return;
                }
                QJsonObject object = jsonDocument.object();
                if(object.contains("message") && object["message"].toString() == "Nickname already in use")
                {
                    editProfile->setDisabled(true);
                    nicknameEdit->setProperty("error", true);
                    nicknameEdit->style()->unpolish(nicknameEdit);
                    nicknameEdit->style()->polish(nicknameEdit);
                }
                else if(object.contains("message") && object["message"].toString() == "Nickname is free")
                {
                    editProfile->setEnabled(true);
                    nicknameEdit->setProperty("error", false);
                    nicknameEdit->style()->unpolish(nicknameEdit);
                    nicknameEdit->style()->polish(nicknameEdit);
                }
            });
            QUrl check_nick(Globals::getInstance().getSERVER_LINK() + "check_nickname/" + nicknameEdit->text());
            client_donwnload_profile_data->getRequest(check_nick);
        }
        else {
            nicknameEdit->setProperty("error", true);
            nicknameEdit->style()->unpolish(nicknameEdit);
            nicknameEdit->style()->polish(nicknameEdit);
            editProfile->setDisabled(true);
        }
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
        nicknameEdit->setText(nicknameLabel->text());
        nameEdit->setText(nameLabel->text());
        surnameEdit->setText(surnameLabel->text());
        nicknameEdit->setProperty("error", false);
        nicknameEdit->style()->unpolish(nicknameEdit);
        nicknameEdit->style()->polish(nicknameEdit);
        editProfile->setEnabled(true);

        newDataMap->insert(0, nameLabel->text());
        newDataMap->insert(1, surnameLabel->text());
        newDataMap->insert(2, nicknameLabel->text());
        newDataMap->insert(3, encoded_photo);

        editProfile->setText(tr("Save"));
        goBackButton->setText(tr("Cancel"));
    }
    else
    {
        nameLabel->setText(oldDataMap->value(0));
        surnameLabel->setText(oldDataMap->value(1));
        nicknameLabel->setText(oldDataMap->value(2));
        encoded_photo = oldDataMap->value(3);
        profilePhoto->setPixmap(VChatWidget::cut_photo(Globals::getInstance().decodeBase64ToPixmap(encoded_photo), 100));
        editProfile->setText(tr("Edit"));
        goBackButton->setText(tr("Back"));
    }
}

void MyProfile::saveChanges()
{
    overlay->showOverlay();
    QJsonObject jsonData;
    QUrl putRequestLink(Globals::getInstance().getSERVER_LINK() + "edit_profile/" + Globals::getInstance().getUserId());
    connect(client_donwnload_profile_data, &HttpClient::responseReceived, this, &MyProfile::handleProfileEditing);

    if(oldDataMap->value(0) != nameEdit->text())
    {
        oldDataMap->insert(0, nameEdit->text());
        nameLabel->setText(nameEdit->text());
        jsonData["name"] = oldDataMap->value(0);
    }
    if(oldDataMap->value(1) != surnameEdit->text())
    {
        oldDataMap->insert(1, surnameEdit->text());
        surnameLabel->setText(surnameEdit->text());
        jsonData["surname"] = oldDataMap->value(1);
    }
    if(oldDataMap->value(2) != nicknameEdit->text())
    {
        oldDataMap->insert(2, nicknameEdit->text());
        nicknameLabel->setText(nicknameEdit->text());
        jsonData["nickname"] = oldDataMap->value(2);
    }
    if(oldDataMap->value(3) != encoded_photo)
    {
        oldDataMap->insert(3, encoded_photo);
        emit profile_photo_changed(Globals::getInstance().decodeBase64ToPixmap(encoded_photo));
    }
    jsonData["profile_photo"] = encoded_photo;
    if(!jsonData.isEmpty())
    {
        qDebug()<<jsonData;
        client_donwnload_profile_data->putRequest(putRequestLink, jsonData);
    }
    else
    {
        overlay->hideOverlay();
    }
}
