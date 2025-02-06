#include "registration.h"
#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "globals.h"
#include "httpclient.h"

Registration::Registration(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    for (int i = 0; i < 8; ++i) {
        valids[i] = false;
    }
    valids[2] = true;
    client_registration = new HttpClient();
    centralWidget = new QWidget(this);
    centralWidget->resize(this->size());

    mainLayout = new QVBoxLayout;

    topLabel = new QLabel;
    topLabel->setStyleSheet("QLabel { font-size: 20px; }");
    topLabel->move(150, 200);
    topLabel->setAlignment(Qt::AlignCenter);

    topLayout = new QVBoxLayout;
    middleLayout = new QHBoxLayout;
    bottomLayout = new QHBoxLayout;

    topLayout->addWidget(topLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    topLayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    formLayout = new QFormLayout;

    QRegularExpression nameRegex("^[a-zA-Z]{2,}$");
    QRegularExpression surnameRegex("^[a-zA-Z]{1,}$");
    QRegularExpression nicknameRegex("^[a-zA-Z][a-zA-Z0-9_]{2,}$");
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    QRegularExpression passwordRegex(
        "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[!@#$%^&*(),.?\":{}|<>]).{6,}$");
    QRegularExpression confirmPasswordRegex(
        "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[!@#$%^&*(),.?\":{}|<>]).{6,}$");

    nameField = new QLineEdit;
    surnameField = new QLineEdit;
    nicknameField = new QLineEdit;
    emailField = new QLineEdit;
    passwordField = new QLineEdit;
    confirmPasswordField = new QLineEdit;
    passwordField->setEchoMode(QLineEdit::Password);
    confirmPasswordField->setEchoMode(QLineEdit::Password);

    nameLabel = new QLabel;
    surnameLabel = new QLabel;
    nicknameLabel = new QLabel;
    emailLabel = new QLabel;
    dateLabel = new QLabel;
    passwordLabel = new QLabel;
    confirmPasswordLabel = new QLabel;

    checkBox = new QCheckBox;
    checkBox->setStyleSheet("margin-top: 0px; margin-bottom: 0px;");
    checkBox->setStyleSheet("QCheckBox::indicator { background-color: lightgrey; }");
    checkBox->setStyleSheet("margin-top: 0px; margin-bottom: 0px;");

    termsOfUseButton = new QPushButton("Terms of use");
    termsOfUseButton->setStyleSheet("QPushButton { background-color: transparent; color: blue; "
                                    "border: none; text-decoration: underline; } ");
    termsOfUseButton->setFixedHeight(checkBox->sizeHint().height());
    QFont tfont = termsOfUseButton->font();
    tfont.setPointSize(10);
    termsOfUseButton->setFont(tfont);

    haveAccountButton = new QPushButton();
    QFont font = haveAccountButton->font();
    font.setPointSize(10);
    haveAccountButton->setFont(font);
    haveAccountButton->setStyleSheet("QPushButton { background-color: transparent; color: blue; "
                                     "border: none; text-decoration: underline; }");

    dateEdit = new QDateEdit;
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setMaximumDate(QDate::currentDate().addYears(-18));

    formLayout->addRow(nameLabel, nameField);

    formLayout->addRow(surnameLabel, surnameField);

    formLayout->addRow(nicknameLabel, nicknameField);

    formLayout->addRow(emailLabel, emailField);

    formLayout->addRow(dateLabel, dateEdit);

    formLayout->addRow(passwordLabel, passwordField);

    formLayout->addRow(confirmPasswordLabel, confirmPasswordField);

    formLayout->addRow(termsOfUseButton, checkBox);
    formLayout->setSpacing(28);

    formLayout->addRow(haveAccountButton);
    formLayout->addItem(new QSpacerItem(0, 40));

    formLayout->setContentsMargins(10, 10, 20, 0);
    formLayout->setLabelAlignment(Qt::AlignLeft);

    middleLayout->addLayout(formLayout);

    prevButton = new QPushButton();

    registerButton = new QPushButton();


    bottomLayout->addWidget(prevButton);
    bottomLayout->addWidget(registerButton);
    bottomLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(middleLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();
    centralWidget->setLayout(mainLayout);

    connect(client_registration, &HttpClient::responseReceived, this, &Registration::handleUserId);

    connect(nameField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = nameRegex.match(nameField->text());
        if (match.hasMatch()) {
            valids[0] = true;
            nameField->setStyleSheet("");
        } else {
            valids[0] = false;
            nameField->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(surnameField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = surnameRegex.match(surnameField->text());
        if (match.hasMatch()) {
            valids[1] = true;
            surnameField->setStyleSheet("");
        } else {
            valids[1] = false;
            surnameField->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(nicknameField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = nicknameRegex.match(nicknameField->text());
        if (match.hasMatch()) {
            valids[2] = true;
            nicknameField->setStyleSheet("");
        } else {
            valids[2] = false;
            nicknameField->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(emailField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = emailRegex.match(emailField->text());
        if (match.hasMatch()) {
            valids[3] = true;
            emailField->setStyleSheet("");
        } else {
            valids[3] = false;
            emailField->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(dateEdit, &QDateEdit::dateChanged, this, [=]() {
        QDate selectedDate = dateEdit->date();
        if (selectedDate.addYears(18) <= QDate::currentDate()) {
            valids[4] = true;
            dateLabel->setStyleSheet("");
        } else {
            valids[4] = false;
            dateLabel->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(passwordField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = passwordRegex.match(passwordField->text());
        if (match.hasMatch()) {
            valids[5] = true;
            passwordField->setStyleSheet("");
            qDebug() << "password is ok";
        } else {
            valids[5] = false;
            passwordField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            qDebug() << "password is  not ok";
        }
    });

    connect(confirmPasswordField, &QLineEdit::textChanged, this, [=]() {
        bool isValid = confirmPasswordRegex.match(confirmPasswordField->text()).hasMatch();
        bool isMatching = confirmPasswordField->text() == passwordField->text();
        if (!isValid || !isMatching) {
            valids[6] = false;
            confirmPasswordField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            qDebug() << "confirm password is not ok";
        } else if (isValid && isMatching) {
            valids[6] = true;
            qDebug() << "confirm password is ok";
            confirmPasswordField->setStyleSheet("");
        }
    });


    connect(haveAccountButton,
            &QPushButton::clicked,
            this,
            &Registration::handle_have_an_account_btn);
    connect(termsOfUseButton, &QPushButton::clicked, this, &Registration::handle_terms_of_use_btn);
    connect(prevButton, &QPushButton::clicked, this, &Registration::handle_prev_btn);
    connect(registerButton, &QPushButton::clicked, this, &Registration::handle_reg_btn);
    connect(checkBox, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        if (state == Qt::Checked) {
            valids[7] = true;
        } else {
            valids[7] = false;
        }
    });
    setLanguage();
    save_texts();
}

void Registration::handle_reg_btn()
{
    bool allValid = true;
    for (int i = 0; i < 8; ++i) {
        if (valids[i] == false) {
            allValid = false;
            break;
        }
    }
    if (allValid) {
        QUrl url("https://synergy-iauu.onrender.com/register/");
        QJsonObject jsonData;
        jsonData["name"] = nameField->text();
        jsonData["surname"] = surnameField->text();
        jsonData["nickname"] = nicknameField->text();
        jsonData["email"] = emailField->text();
        jsonData["date_of_birth"] = dateEdit->text();
        jsonData["password"] = passwordField->text();

        qDebug() << jsonData.keys();
        qDebug() << jsonData.value("name");
        qDebug() << jsonData.value("nickname");
        qDebug() << jsonData.value("password");
        qDebug() << jsonData.value("surname");
        qDebug() << jsonData.value("email");
        qDebug() << jsonData.value("date_of_birth");

        client_registration->postRequest(url, jsonData);
    }
}

void Registration::handle_prev_btn()
{
    emit prev_btn_signal();
}

void Registration::handle_have_an_account_btn()
{
    emit have_an_account_signal();
}

void Registration::handle_terms_of_use_btn()
{
    emit terms_of_use_signal();
}

void Registration::setLanguage()
{
    topLabel->setText(tr("Registration"));
    nameField->setPlaceholderText(tr("John"));
    surnameField->setPlaceholderText(tr("Doe"));
    emailField->setPlaceholderText(tr("johndoe@example.com"));

    nicknameField->setPlaceholderText(tr("Enter your nickname"));
    passwordField->setPlaceholderText(tr("Enter your password"));
    confirmPasswordField->setPlaceholderText(tr("Reenter your password"));
    nameLabel->setText(tr("First name"));
    surnameLabel->setText(tr("Last name (optional)"));
    nicknameLabel->setText(tr("Nickname"));
    emailLabel->setText(tr("Email Address"));
    dateLabel->setText(tr("Date of Birth"));
    passwordLabel->setText(tr("Password"));
    confirmPasswordLabel->setText(tr("Confirm password"));
    termsOfUseButton->setText(tr("Terms of use"));
    haveAccountButton->setText(tr("Already have an account?"));
    prevButton->setText(tr("Back"));
    registerButton->setText(tr("Register"));
}

void Registration::save_texts()
{
    name_txt = nameField->text();
    nickname_txt = nicknameField->text();
    surname_txt = surnameField->text();
    date_txt = dateEdit->text();
    email_txt = emailField->text();
    password_txt = passwordField->text();
}

void Registration::handleUserId(QByteArray responseData)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();


    if (jsonObject.contains("user_id"))
    {
        Globals::getInstance().setUserID(jsonObject["user_id"].toString());
        qDebug() << Globals::getInstance().getUserId();
        emit idreceived();
        emit reg_btn_signal();
    }
    else if(jsonObject.contains("detail"))
    {
        if(jsonResponse["detail"].toString() == "Nickname already in use")
        {
            qDebug()<<"Debug" <<"Nickname already in use";
            nicknameField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            emailField->setStyleSheet("");
        }
        else if(jsonResponse["detail"].toString() == "Email already in use")
        {
            qDebug()<<"Debug" <<"Email already in use";
            emailField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            nicknameField->setStyleSheet("");
        }
    }
}

Registration::~Registration() {}
