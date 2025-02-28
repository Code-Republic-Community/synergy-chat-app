#include "registration.h"
#include "globals.h"
#include "httpclient.h"

#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStyle>

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

    QWidget *topLayoutContainer = new QWidget(this);
    topLayoutContainer->setGeometry(0, 0, 400, 150);
    QVBoxLayout *topLayout = new QVBoxLayout(topLayoutContainer);
    topLabel = new QLabel(tr("Registration"), this);
    topLabel->setObjectName("topLabel");
    topLabel->setAlignment(Qt::AlignHCenter);
    smallText = new QLabel(tr("Welcome to DeltaSynergy! A place for meaningful") + "\n" +
                           tr("conversations. Connect with your friends and family,") + "\n" +
                           tr("build your community, and deepen your interests."), this);
    smallText->setObjectName("smallText");
    smallText->setWordWrap(true);
    smallText->setAlignment(Qt::AlignHCenter);
    topLayout->addWidget(topLabel);
    topLayout->addWidget(smallText);
    topLayout->addStretch();
    QWidget *formLayoutContainer = new QWidget(this);
    formLayoutContainer->setGeometry(0, 150, 400, 500);
    formLayout = new QFormLayout(formLayoutContainer);

    nameField = new QLineEdit(this);
    surnameField = new QLineEdit(this);
    nicknameField = new QLineEdit(this);
    emailField = new QLineEdit(this);
    passwordField = new QLineEdit(this);
    confirmPasswordField = new QLineEdit(this);

    passwordField->setEchoMode(QLineEdit::Password);
    confirmPasswordField->setEchoMode(QLineEdit::Password);

    nameLabel = new QLabel(this);
    nameLabel->setObjectName("nameLabel");
    surnameLabel = new QLabel(this);
    surnameLabel->setObjectName("surnameLabel");
    nicknameLabel = new QLabel(this);
    nicknameLabel->setObjectName("nicknameLabel");
    emailLabel = new QLabel(this);
    emailLabel->setObjectName("emailLabel");
    dateLabel = new QLabel(this);
    dateLabel->setObjectName("dateLabel");
    passwordLabel = new QLabel(this);
    passwordLabel->setObjectName("passwordLabel");
    confirmPasswordLabel = new QLabel(this);
    confirmPasswordLabel->setObjectName("confirmPasswordLabel");
    checkBox = new QCheckBox(this);
    checkBox->setObjectName("checkBox");

    termsOfUseButton = new QLabel(tr("Terms of use"));
    termsOfUseButton->setObjectName("termsOfUseButton");
    termsOfUseButton->setCursor(Qt::PointingHandCursor);
    haveAccountButton = new QPushButton();
    dateEdit = new QDateEdit;

    haveAnAccountLabel = new QLabel(tr("Already have an account?"));
    haveAnAccountClickLabel = new QLabel(tr("Login"));
    haveAnAccountClickLabel->setObjectName("haveAnAccountClickLabel");
    haveAnAccountClickLabel->setCursor(Qt::PointingHandCursor);

    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");

    formLayout->setSpacing(12);

    formLayout->addRow(nameLabel, nameField);

    formLayout->addRow(surnameLabel, surnameField);

    formLayout->addRow(nicknameLabel, nicknameField);

    formLayout->addRow(emailLabel, emailField);

    formLayout->addRow(dateLabel, dateEdit);

    formLayout->addRow(passwordLabel, passwordField);

    formLayout->addRow(confirmPasswordLabel, confirmPasswordField);

    formLayout->addRow(termsOfUseButton, checkBox);

    formLayout->addItem(new QSpacerItem(0, 5));

    formLayout->addRow(haveAnAccountLabel, haveAnAccountClickLabel);

    formLayout->setContentsMargins(10, 10, 20, 0);

    formLayout->setLabelAlignment(Qt::AlignLeft);

    QWidget *bottomLayoutContainer = new QWidget(this);
    bottomLayoutContainer->setGeometry(0, 600, 400, 100);
    bottomLayout = new QHBoxLayout(bottomLayoutContainer);
    prevButton = new QPushButton(this);
    registerButton = new QPushButton(this);
    QSize minSize(100, 40);
    prevButton->setMinimumSize(minSize);
    registerButton->setMinimumSize(minSize);
    bottomLayout->addWidget(prevButton);
    bottomLayout->addWidget(registerButton);
    bottomLayout->setAlignment(Qt::AlignHCenter);

    setLanguage();

    QRegularExpression nameRegex("^[a-zA-Z]{2,}$");
    QRegularExpression surnameRegex("^[a-zA-Z]{1,}$");
    QRegularExpression nicknameRegex("^[a-zA-Z][a-zA-Z0-9_]{2,}$");
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[!@#$%^&*(),.?\":{}|<>]).{6,}$");


    connect(client_registration, &HttpClient::responseReceived, this, &Registration::handleUserId);

    connect(nameField, &QLineEdit::textChanged, this, [=]() {
        if (nameRegex.match(nameField->text()).hasMatch()) {
            valids[0] = true;
            nameField->setProperty("error", false);
            nameField->style()->unpolish(nameField);
            nameField->style()->polish(nameField);
        } else {
            valids[0] = false;
            nameField->setProperty("error", true);
            nameField->style()->unpolish(nameField);
            nameField->style()->polish(nameField);
        }
    });

    connect(surnameField, &QLineEdit::textChanged, this, [=]() {
        if (surnameRegex.match(surnameField->text()).hasMatch()) {
            valids[1] = true;
            surnameField->setProperty("error", false);
            surnameField->style()->unpolish(surnameField);
            surnameField->style()->polish(surnameField);
        } else {
            valids[1] = false;
            surnameField->setProperty("error", true);
            surnameField->style()->unpolish(surnameField);
            surnameField->style()->polish(surnameField);
        }
    });

    connect(nicknameField, &QLineEdit::textChanged, this, [=]() {
        if (nicknameRegex.match(nicknameField->text()).hasMatch()) {
            valids[2] = true;
            nicknameField->setProperty("error", false);
            nicknameField->style()->unpolish(nicknameField);
            nicknameField->style()->polish(nicknameField);
        } else {
            valids[2] = false;
            nicknameField->setProperty("error", true);
            nicknameField->style()->unpolish(nicknameField);
            nicknameField->style()->polish(nicknameField);
        }
    });

    connect(emailField, &QLineEdit::textChanged, this, [=]() {
        if (emailRegex.match(emailField->text()).hasMatch()) {
            valids[3] = true;
            emailField->setProperty("error", false);
            emailField->style()->unpolish(emailField);
            emailField->style()->polish(emailField);
        } else {
            valids[3] = false;
            emailField->setProperty("error", true);
            emailField->style()->unpolish(emailField);
            emailField->style()->polish(emailField);
        }
    });

    connect(dateEdit, &QDateEdit::dateChanged, this, [=]() {
        QDate selectedDate = dateEdit->date();
        if (selectedDate.addYears(18) <= QDate::currentDate()) {
            valids[4] = true;
            dateLabel->setProperty("error", false);
            dateLabel->style()->unpolish(dateLabel);
            dateLabel->style()->polish(dateLabel);
        } else {
            valids[4] = false;
            dateLabel->setStyleSheet("QLineEdit { border: 2px solid red; }");
            dateLabel->setProperty("error", true);
            dateLabel->style()->unpolish(dateLabel);
            dateLabel->style()->polish(dateLabel);
        }
    });

    connect(passwordField, &QLineEdit::textChanged, this, [=]() {
        if (passwordRegex.match(passwordField->text()).hasMatch()) {
            valids[5] = true;
            passwordField->setProperty("error", false);
            passwordField->style()->unpolish(passwordField);
            passwordField->style()->polish(passwordField);
            qDebug() << "password is ok";
        } else {
            valids[5] = false;
            passwordField->setProperty("error", true);
            passwordField->style()->unpolish(passwordField);
            passwordField->style()->polish(passwordField);
            qDebug() << "password is  not ok";
        }
    });

    connect(confirmPasswordField, &QLineEdit::textChanged, this, [=]() {
        bool isMatching = confirmPasswordField->text() == passwordField->text();
        if (!isMatching) {
            valids[6] = false;
            confirmPasswordField->setProperty("error", true);
            confirmPasswordField->style()->unpolish(confirmPasswordField);
            confirmPasswordField->style()->polish(confirmPasswordField);
            qDebug() << "confirm password is not ok";
        } else if (isMatching) {
            valids[6] = true;
            qDebug() << "confirm password is ok";
            confirmPasswordField->setProperty("error", false);
            confirmPasswordField->style()->unpolish(confirmPasswordField);
            confirmPasswordField->style()->polish(confirmPasswordField);
        }
    });

    connect(prevButton, &QPushButton::clicked, this, &Registration::handle_prev_btn);
    connect(registerButton, &QPushButton::clicked, this, &Registration::handle_reg_btn);
    connect(checkBox, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        if (state == Qt::Checked) {
            valids[7] = true;
            termsOfUseButton->setProperty("error", false);
            termsOfUseButton->style()->unpolish(termsOfUseButton);
            termsOfUseButton->style()->polish(termsOfUseButton);
        } else {
            valids[7] = false;
            termsOfUseButton->setProperty("error", true);
            termsOfUseButton->style()->unpolish(termsOfUseButton);
            termsOfUseButton->style()->polish(termsOfUseButton);
        }
    });
    save_texts();
}

void Registration::handle_reg_btn()
{
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


void Registration::setLanguage()
{
    smallText->setText(tr("Welcome to Synergy Chat App! A place for meaningful") + "\n" +
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

    haveAnAccountLabel->setText(tr("Already have an account?"));
    haveAnAccountClickLabel->setText(tr("Login"));

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

void Registration::mousePressEvent(QMouseEvent *event)
{
    if (haveAnAccountClickLabel->underMouse())
    {
        qDebug() << "Login link clicked!";
        handle_have_an_account_btn();
    }
    if (termsOfUseButton->underMouse())
    {
        QMessageBox::information(
            this, tr("Terms of use"),
            tr("These terms of use is an agreement between Synergy Chat and you that sets forth the general terms and conditions of your use of any Synergys products or services."));
    }
    if (event)
    {
    }
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
        overlay->hideOverlay();
        emit reg_btn_signal();
    }
    else if (jsonObject.contains("detail")) {
        if (jsonResponse["detail"].toString() == "Nickname already in use") {
            qDebug() << "Debug" << "Nickname already in use";
            emailField->setProperty("error", false);
            emailField->style()->unpolish(emailField);
            emailField->style()->polish(emailField);
            nicknameField->setProperty("error", true);
            nicknameField->style()->unpolish(nicknameField);
            nicknameField->style()->polish(nicknameField);
        } else if (jsonResponse["detail"].toString() == "Email already in use") {
            qDebug() << "Debug" << "Email already in use";
            emailField->setProperty("error", true);
            emailField->style()->unpolish(emailField);
            emailField->style()->polish(emailField);
            nicknameField->setProperty("error", false);
            nicknameField->style()->unpolish(nicknameField);
            nicknameField->style()->polish(nicknameField);
        }
        overlay->hideOverlay();
    }
}

Registration::~Registration() {}

void Registration::clear_fields()
{
    nameField->setText("");
    surnameField->setText("");
    emailField->setText("");
    nicknameField->setText("");
    passwordField->setText("");
    confirmPasswordField->setText("");
    checkBox->setCheckState(Qt::Unchecked);

    nicknameField->setProperty("error", false);
    nicknameField->style()->unpolish(nicknameField);
    nicknameField->style()->polish(nicknameField);
    nameField->setProperty("error", false);
    nameField->style()->unpolish(nameField);
    nameField->style()->polish(nameField);
    surnameField->setProperty("error", false);
    surnameField->style()->unpolish(surnameField);
    surnameField->style()->polish(surnameField);
    emailField->setProperty("error", false);
    emailField->style()->unpolish(emailField);
    emailField->style()->polish(emailField);
    dateLabel->setProperty("error", false);
    dateLabel->style()->unpolish(dateLabel);
    dateLabel->style()->polish(dateLabel);
    passwordField->setProperty("error", false);
    passwordField->style()->unpolish(passwordField);
    passwordField->style()->polish(passwordField);
    confirmPasswordField->setProperty("error", false);
    confirmPasswordField->style()->unpolish(confirmPasswordField);
    confirmPasswordField->style()->polish(confirmPasswordField);
    termsOfUseButton->setProperty("error", false);
    termsOfUseButton->style()->unpolish(termsOfUseButton);
    termsOfUseButton->style()->polish(termsOfUseButton);
}
