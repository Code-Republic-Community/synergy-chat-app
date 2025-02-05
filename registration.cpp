#include "registration.h"
#include "httpclient.h"
#include <QDate>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

extern QByteArray globalResponse;

Registration::Registration(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    client_login = new HttpClient();
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
    QRegularExpression emailRegex("^[\\w\\.]+@[\\w\\.]+\\.[a-z]{2,}$");
    QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[!@#$%^&*(),.?\":{}|<>]).{6,}$");
    QRegularExpression confirmPasswordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[!@#$%^&*(),.?\":{}|<>]).{6,}$");

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
    dateEdit->setDisplayFormat("dd/MM/yyyy");
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
    mainLayout ->addStretch();
    mainLayout->addLayout(topLayout);
    mainLayout ->addStretch();
    mainLayout->addLayout(middleLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    mainLayout ->addStretch();
    centralWidget->setLayout(mainLayout);

    connect(nameField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = nameRegex.match(nameField->text());
        if (match.hasMatch())
        {
            nameField->setStyleSheet("");
        }
        else
        {
            nameField-> setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(surnameField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = surnameRegex.match(surnameField->text());
        if (match.hasMatch())
        {
            surnameField->setStyleSheet("");
        }
        else
        {
            surnameField->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(nicknameField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = nicknameRegex.match(nicknameField->text());
        if (match.hasMatch())
        {
            nicknameField ->setStyleSheet("");
        }
        else
        {
            nicknameField ->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(emailField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = emailRegex.match(emailField->text());
        if (match.hasMatch())
        {
            emailField ->setStyleSheet("");
        }
        else
        {
            emailField -> setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
    });

    connect(dateEdit, &QDateEdit::dateChanged, this, [=]() {
        QDate selectedDate = dateEdit->date();
        if (selectedDate.addYears(18) > QDate::currentDate())
        {
            dateLabel->setStyleSheet("QLineEdit { border: 2px solid red; }");
        }
        else
        {
            dateLabel->setStyleSheet("");
        }
    });

    connect(passwordField, &QLineEdit::textChanged, this, [=]() {
        QRegularExpressionMatch match = passwordRegex.match(passwordField->text());
        if (match.hasMatch())
        {
            passwordField->setStyleSheet("");
            qDebug()<<"password is ok";
        }
        else
        {
            passwordField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            qDebug()<<"password is  not ok";
        }
    });

    connect(confirmPasswordField, &QLineEdit::textChanged, this, [=]() {
        bool isValid = confirmPasswordRegex.match(confirmPasswordField->text()).hasMatch();
        bool isMatching = confirmPasswordField->text() == passwordField->text();
        if (!isValid || !isMatching) {
            confirmPasswordField->setStyleSheet("QLineEdit { border: 2px solid red; }");
            qDebug()<<"confirm password is ok";
        }
        else if(isValid && isMatching)
        {
            qDebug()<<"confirm password is ok";
            confirmPasswordField->setStyleSheet("");
        }
    });

    connect(haveAccountButton, &QPushButton::clicked, this, &Registration::handle_have_an_account_btn);
    connect(termsOfUseButton, &QPushButton::clicked, this, &Registration::handle_terms_of_use_btn);
    connect(prevButton, &QPushButton::clicked, this, &Registration::handle_prev_btn);
    connect(registerButton, &QPushButton::clicked, this, &Registration::handle_reg_btn);
    setLanguage();
    save_texts();
}


void Registration::handle_reg_btn()
{
    bool allValid = true;
    for (int i = 0; i < formLayout->rowCount(); ++i) {
        QLabel *label = qobject_cast<QLabel *>(
            formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        if (!label) {
            continue;
        }
        if (!label->text().contains("!"))
        {
            allValid = false;
            break;
        }
        if (registerButton && !registerButton->isEnabled())
        {
            registerButton->setEnabled(allValid);
        }

        if (allValid) {
            QUrl url("http://127.0.0.1:8000/register/");
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

            if (globalResponse != "") {
                client_login->postRequest(url, jsonData);
            } else {
                qDebug() << "Error";
            }

            emit reg_btn_signal();
        }
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

Registration::~Registration() {}
