#include "registration.h"
#include "globals.h"
#include "httpclient.h"

#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

Registration::Registration(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    for (int i = 0; i < 8; ++i) {
        valids[i] = false;
    }
    valids[1] = true;
    valids[4] = true;
    client_registration = new HttpClient();
    overlay = new LoadingOverlay(this);

    centralWidget = new QWidget(this);


    mainLayout = new QVBoxLayout;

    topLabel = new QLabel(this);
    topLabel->setStyleSheet("QLabel { font-size: 20px; }");
    topLabel->move(150, 50);
    topLabel->setAlignment(Qt::AlignCenter);

    smallText = new QLabel(this);
    smallText->setGeometry(50, 30 + topLabel->height(), 300, 100);
    smallText->setStyleSheet("QLabel { font-size: 10px; }");
    smallText->setWordWrap(true);
    smallText->setAlignment(Qt::AlignCenter);

    centralWidget->setGeometry(0, smallText->y() + smallText->height(), 400, 700 - smallText->y()-smallText->height());

    topLayout = new QVBoxLayout;
    middleLayout = new QHBoxLayout;
    bottomLayout = new QHBoxLayout;

    formLayout = new QFormLayout;

    QRegularExpression nameRegex("^[a-zA-Z]{2,}$");
    QRegularExpression surnameRegex("^[a-zA-Z]{1,}$");
    QRegularExpression nicknameRegex("^[a-zA-Z][a-zA-Z0-9_]{2,}$");
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    QRegularExpression passwordRegex(
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
    dateEdit->setStyleSheet("");

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

    setLanguage();

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
        bool isMatching = confirmPasswordField->text() == passwordField->text();
        if (!isMatching) {
            valids[6] = false;
            confirmPasswordField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            qDebug() << "confirm password is not ok";
        } else if (isMatching) {
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
            termsOfUseButton->setStyleSheet(
                "QPushButton { background-color: transparent; color: blue; "
                "border: none; text-decoration: underline; } ");
            termsOfUseButton->setFixedHeight(checkBox->sizeHint().height());
            QFont tfont = termsOfUseButton->font();
            tfont.setPointSize(10);
            termsOfUseButton->setFont(tfont);
        } else {
            valids[7] = false;
            termsOfUseButton->setStyleSheet(
                "QPushButton { background-color: transparent; color: red; "
                "border: none; text-decoration: underline; } ");
            termsOfUseButton->setFixedHeight(checkBox->sizeHint().height());
            QFont tfont = termsOfUseButton->font();
            tfont.setPointSize(10);
            termsOfUseButton->setFont(tfont);
        }
    });
    connect(termsOfUseButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(
            this, tr("Terms of use"),
            tr("These terms of use is an agreement between Synergy Chat and you that sets forth the general terms and conditions of your use of any Synergys products or services."));
    });
    save_texts();
}

void Registration::handle_reg_btn()
{
    // emit startloading();
    overlay->showOverlay();
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
    else
    {
        overlay->hideOverlay();
        // emit stoploading();
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
    smallText->setText(tr("Welcome to DeltaSynergy! A place for meaningful") + "\n" +
                       tr("conversations. Connect with your friends and family,") + "\n" +
                       tr("build your community, and deepen your interests."));
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

    if (jsonObject.contains("message") &&
        jsonObject["message"].toString() == "Verification code sent. Please verify to complete registration." &&
        jsonObject.contains("user_id")) {

        Globals::getInstance().setUserID(jsonObject["user_id"].toString());
        qDebug() << Globals::getInstance().getUserId();
        emit email_obt_signal(emailField->text());
        emit idreceived();
        // emit stoploading();
        overlay->hideOverlay();
        emit reg_btn_signal();
    }
    else if (jsonObject.contains("detail")) {
        if (jsonResponse["detail"].toString() == "Nickname already in use") {
            qDebug() << "Debug" << "Nickname already in use";
            nicknameField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            emailField->setStyleSheet("");
        } else if (jsonResponse["detail"].toString() == "Email already in use") {
            qDebug() << "Debug" << "Email already in use";
            emailField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            nicknameField->setStyleSheet("");
        }
        overlay->hideOverlay();
        // emit stoploading();
    }
}

Registration::~Registration() {}

void Registration::clear_fields()
{
    nameField->setText("");
    nameField->setStyleSheet("");
    surnameField->setText("");
    surnameField->setStyleSheet("");
    emailField->setText("");
    emailField->setStyleSheet("");
    nicknameField->setText("");
    nicknameField->setStyleSheet("");
    passwordField->setText("");
    passwordField->setStyleSheet("");
    confirmPasswordField->setText("");
    confirmPasswordField->setStyleSheet("");
    checkBox->setCheckState(Qt::Unchecked);
    termsOfUseButton->setStyleSheet("QPushButton { background-color: transparent; color: blue; "
                                    "border: none; text-decoration: underline; } ");
    termsOfUseButton->setFixedHeight(checkBox->sizeHint().height());
    QFont tfont = termsOfUseButton->font();
    tfont.setPointSize(10);
    termsOfUseButton->setFont(tfont);
}
