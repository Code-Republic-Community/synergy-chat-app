#include "otherprofile.h"
#include <QDebug>
#include <QPixmap>

OtherProfile::OtherProfile(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 700);
    this->setFixedSize(400, 700);
    init();
    setup();
    styling();
    connections();
}

void OtherProfile::init()
{
    profilePhoto = new QLabel(this);
    nameLabel = new QLabel(this);
    surnameLabel = new QLabel(this);
    nicknameLabel = new QLabel(this);
    goBackButton = new QPushButton("Back", this);
}

void OtherProfile::setup()
{
    nameLabel->setText("Tigran");
    surnameLabel->setText("Yeghyan");
    nicknameLabel->setText("@tigranyeghyan");

    goBackButton->setGeometry(10, 10, 80, 40);
    profilePhoto->setGeometry(150, 10, 100, 100);

    nameLabel->setGeometry(20, 130, 360, 30);
    surnameLabel->setGeometry(20, 170, 360, 30);
    nicknameLabel->setGeometry(20, 210, 360, 30);

    QPixmap profilePic("C:/Users/tigra/OneDrive/Pictures/user.png");
    if (!profilePic.isNull()) {
        profilePhoto->setPixmap(profilePic.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "Failed to load profile photo. Using default.";
        profilePhoto->setPixmap(QPixmap(":/resources/default_profile.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
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

    QString photoStyle = R"(
        QLabel {
            border: 3px solid #420242;
            border-radius: 50px;
        }
    )";

    profilePhoto->setStyleSheet(photoStyle);
    profilePhoto->setAlignment(Qt::AlignCenter);

    this->setStyleSheet("background-color: #000000;");
}

void OtherProfile::connections()
{
    connect(goBackButton, &QPushButton::clicked, this, [this]() {
        qDebug() <<"go Back Signal";
        emit goBackSignal();
    });
}

