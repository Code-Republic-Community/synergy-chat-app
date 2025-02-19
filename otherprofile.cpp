#include "otherprofile.h"
#include "globals.h"
#include "v_chat_widget.h"

#include <QDebug>
#include <QJsonDocument>
#include <QPixmap>


OtherProfile::OtherProfile(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 700);
    this->setFixedSize(400, 700);
    init();
    setup();
    setLanguage();
    styling();
    connections();
}

void OtherProfile::setLanguage()
{
    qDebug() << "Setting text in Other Proffile";
    goBackButton->setText(tr("Back"));
    if (nameSmallLabel) nameSmallLabel->setText(tr("Name"));
    if (surnameSmallLabel) surnameSmallLabel->setText(tr("Surname"));
    if (nicknameSmallLabel) nicknameSmallLabel->setText(tr("Nickname"));
}

void OtherProfile::handleDataFromChat(QString nickname, QString name, QString surname , QPixmap photo)
{
    disconnect(client_other_profile, &HttpClient::responseReceived, nullptr, nullptr);
    nameLabel->setText(name);
    surnameLabel->setText(surname);
    nicknameLabel->setText("@" + nickname);
    profilePhoto ->setPixmap(VChatWidget::cut_photo(photo, 100));
    connect(client_other_profile, &HttpClient::responseReceived, this, [=](QByteArray responseData) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isObject()) {
            qDebug() << "Invalid JSON response in get_contacts_info_and_show";
            return;
        }

        QJsonObject jsonObject = jsonDoc.object();
        QString _name = jsonObject.value("name").toString();
        QString _surname = jsonObject.value("surname").toString();
        QString _nickname = jsonObject.value("nickname").toString();
        QString _encodedphoto = jsonObject.value("profile_photo").toString();
        QPixmap _decodedphoto(Globals::getInstance().decodeBase64ToPixmap(_encodedphoto));
        nameLabel->setText(_name);
        surnameLabel->setText(_surname);
        nicknameLabel->setText("@" + _nickname);
        profilePhoto->setPixmap(VChatWidget::cut_photo(_decodedphoto, 100));
    });
    QString contactInfoGetRequestLink = "https://synergy-iauu.onrender.com/getContactInfo/"
                                        + Globals::getInstance().getUserId() + "/" + nickname;
    client_other_profile->getRequest(contactInfoGetRequestLink);

}

void OtherProfile::init()
{
    client_other_profile = new HttpClient;
    profilePhoto = new QLabel(this);
    createLabelPair(tr("Name"), nameSmallLabel, nameLabel);
    createLabelPair(tr("Surname"), surnameSmallLabel, surnameLabel);
    createLabelPair(tr("Nickname"), nicknameSmallLabel, nicknameLabel);
    goBackButton = new QPushButton(this);
}

void OtherProfile::setup()
{
    nameLabel->setText("John");
    surnameLabel->setText("Doe");
    nicknameLabel->setText("@johndoe");

    setupLabelPosition(nameSmallLabel, nameLabel, 150);
    setupLabelPosition(surnameSmallLabel, surnameLabel, 210);
    setupLabelPosition(nicknameSmallLabel, nicknameLabel, 270);


    goBackButton->setGeometry(10, 10, 80, 40);
    profilePhoto->setGeometry(150, 10, 100, 100);

    profilePhoto->setPixmap(VChatWidget::cut_photo(QPixmap(":/pngs/panda.jpg"), 100));
}

void OtherProfile::createLabelPair(const QString &smallText, QLabel *&smallLabel, QLabel *&mainLabel)
{
    smallLabel = new QLabel(smallText, this);
    smallLabel->setStyleSheet("font-size: 10px; color: white;");

    mainLabel = new QLabel(this);
    mainLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
}

void OtherProfile::setupLabelPosition(QLabel *smallLabel, QLabel *mainLabel, int y)
{

    smallLabel->setGeometry(10, y - 17, 380, 15);
    mainLabel->setGeometry(10, y, 380, 30);
}

void OtherProfile::styling()
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

    QString buttonStyle = R"(
        QPushButton {
            border: 1px solid #420242;
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
    goBackButton->setFocusPolicy(Qt::NoFocus);
}

void OtherProfile::connections()
{
    connect(goBackButton, &QPushButton::clicked, this, [this]() {
        emit goBackSignal();
    });
}
