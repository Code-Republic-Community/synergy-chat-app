#include "registration.h"
#include "httpclient.h"

#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QMessageBox>
#include <QJsonObject>
#include <QDate>


extern QByteArray globalResponse;

Registration::Registration(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    //init();
    client_login = new HttpClient();
    //}
    //void Registration::init {
    centralWidget = new QWidget(this);

    this->setStyleSheet("background-color: grey;");

    mainLayout = new QVBoxLayout;

    topLabel = new QLabel;
    topLabel->setText("<h1>Registration</h1>");
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
    QRegularExpression passwordRegex("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");
    QRegularExpression confirmPasswordRegex("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");

    nameField = new QLineEdit;
    nameField->setPlaceholderText("John");
    //nameField->setValidator(new QRegularExpressionValidator(nameRegex, this));
    surnameField = new QLineEdit;
    surnameField->setPlaceholderText("Smidth");
    //surnameField->setValidator(new QRegularExpressionValidator(surnameRegex, this));
    nicknameField = new QLineEdit;
    nicknameField->setPlaceholderText("Enter your nickname");
    //nicknameField->setValidator(new QRegularExpressionValidator(nicknameRegex, this));
    emailField = new QLineEdit;
    emailField->setPlaceholderText("john.smidth@yahoo.com");
    // emailField->setValidator(new QRegularExpressionValidator(emailRegex, this));
    passwordField = new QLineEdit;
    passwordField->setPlaceholderText("Enter your password");
    // passwordField->setValidator(new QRegularExpressionValidator(passwordRegex, this));
    confirmPasswordField = new QLineEdit;
    confirmPasswordField->setPlaceholderText("Reenter your password");
    passwordField->setEchoMode(QLineEdit::Password);
    // confirmPasswordField->setValidator(new QRegularExpressionValidator(confirmPasswordRegex, this));
    confirmPasswordField->setEchoMode(QLineEdit::Password);

    nameLabel = new QLabel;
    nameLabel->setText("Name:");
    surnameLabel = new QLabel;
    surnameLabel->setText("Last name (optional):");
    nicknameLabel = new QLabel;
    nicknameLabel->setText("Nickname:");
    emailLabel = new QLabel;
    emailLabel->setText("Email address:");
    dateLabel = new QLabel;
    dateLabel->setText("Date of birth:");
    passwordLabel = new QLabel;
    passwordLabel->setText("Password:");
    confirmPasswordLabel = new QLabel;
    confirmPasswordLabel->setText("Confirm password:");

    checkBox = new QCheckBox;
    checkBox->setStyleSheet("margin-top: 0px; margin-bottom: 0px;");
    checkBox->setStyleSheet("QCheckBox::indicator { background-color: lightgrey; }");
    checkBox->setStyleSheet("margin-top: 0px; margin-bottom: 0px;");

    termsOfUseButton = new QPushButton("Terms of use");
    termsOfUseButton->setStyleSheet("QPushButton { background-color: transparent; color: blue; border: none; text-decoration: underline; } ");
    termsOfUseButton->setFixedHeight(checkBox->sizeHint().height());
    QFont tfont = termsOfUseButton->font();
    tfont.setPointSize(10);
    termsOfUseButton->setFont(tfont);

    haveAccountButton = new QPushButton("Already have an account?");
    QFont font = haveAccountButton->font();
    font.setPointSize(10);
    haveAccountButton->setFont(font);
    haveAccountButton->setStyleSheet("QPushButton { background-color: transparent; color: blue; border: none; text-decoration: underline; }");

    dateEdit = new QDateEdit;
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setMaximumDate(QDate::currentDate().addYears(-18));

    formLayout->addRow("First name:", nameField);
    nameField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

    formLayout->addRow("Last name (optional):", surnameField);
    surnameField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

    formLayout->addRow("Nickname:", nicknameField);
    nicknameField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

    formLayout->addRow("Email address:", emailField);
    emailField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

    formLayout->addRow("Date of birth:", dateEdit);
    dateEdit->setStyleSheet("QDateEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

    formLayout->addRow("Password:", passwordField);
    passwordField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

    formLayout->addRow("Confirm password:", confirmPasswordField);
    confirmPasswordField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

    formLayout->addRow(termsOfUseButton, checkBox);
    formLayout->setSpacing(28);

    formLayout->addRow(nullptr, haveAccountButton);
    formLayout->addItem(new QSpacerItem(0, 40));

    formLayout->setContentsMargins(10, 10, 20, 0);
    formLayout->setLabelAlignment(Qt::AlignLeft);

    middleLayout->addLayout(formLayout);

    prevButton = new QPushButton("< Previous");
    prevButton->setStyleSheet("QPushButton { background-color: grey; color: black; border: 2px solid darkgrey; border-radius: 8px;}");
    prevButton->setStyleSheet("QPushButton { width: 90px; height: 40px; }");

    registerButton = new QPushButton("Register >");
    //registerButton->setEnabled(false);
    registerButton->setStyleSheet("QPushButton { background-color: green; color: black; width: 100px; height: 40px; border: 2px solid green; border-radius: 8px;}");
    registerButton->setStyleSheet("QPushButton { width: 90px; height: 40px; }");

    bottomLayout->addWidget(prevButton);
    bottomLayout->addWidget(registerButton);
    bottomLayout->setAlignment(Qt::AlignHCenter);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(bottomLayout);

    centralWidget->setLayout(mainLayout);

    connect(nameField, &QLineEdit::textChanged, this, [=]() {
        QString defaultText = nameLabel->text();
        QRegularExpressionMatch match = nameRegex.match(nameField->text());
        if(!match.hasMatch()) {
            if(!defaultText.contains("!")) {
                nameLabel->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            nameLabel->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }
    });

    connect(surnameField, &QLineEdit::textChanged, this, [=]() {
        QString defaultText = surnameLabel->text();
        QRegularExpressionMatch match = surnameRegex.match(surnameField->text());
        if(!match.hasMatch()) {
            if(!defaultText.contains("!")) {
                surnameLabel->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            surnameLabel->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }
    });

    connect(nicknameField, &QLineEdit::textChanged, this, [=]() {
        QString defaultText = nicknameLabel->text();
        QRegularExpressionMatch match = nicknameRegex.match(nicknameField->text());
        if(!match.hasMatch()) {
            if(!defaultText.contains("!")) {
                nicknameLabel->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            nicknameLabel->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }
    });

    connect(emailField, &QLineEdit::textChanged, this, [=]() {
        QString defaultText = emailLabel->text();
        QRegularExpressionMatch match = emailRegex.match(emailField->text());
        if(!match.hasMatch()) {
            if(!defaultText.contains("!")) {
                emailLabel->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            emailLabel->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }

    });

    connect(dateEdit, &QDateEdit::dateChanged, this, [=]() {
        QString defaultText = dateLabel->text();
        QDate selectedDate = dateEdit->date();
        if (selectedDate.addYears(18) > QDate::currentDate()) {
            if (!defaultText.contains("!")) {
                dateLabel->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            dateLabel->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }
    });

    connect(passwordField, &QLineEdit::textChanged, this, [=]() {
        QString defaultText = passwordLabel->text();
        QRegularExpressionMatch match = passwordRegex.match(passwordField->text());
        if(!match.hasMatch()) {
            if(!defaultText.contains("!")) {
                passwordLabel->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            passwordLabel->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }
    });

    connect(confirmPasswordField, &QLineEdit::textChanged, this, [=]() {
        QString defaultText = confirmPasswordLabel->text();
        bool isValid = confirmPasswordRegex.match(confirmPasswordField->text()).hasMatch();
        bool isMatching = confirmPasswordField->text() == passwordField->text();
        if(!isValid || !isMatching) {
            if(!defaultText.contains("!")) {
                confirmPasswordLabel->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            confirmPasswordLabel->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }
    });

    connect(checkBox, &QCheckBox::clicked, this, [=]() {
        QString defaultText = termsOfUseButton->text();
        if(!checkBox->isChecked()) {
            if(!termsOfUseButton->text().contains(" <span style='color: red;'>!</span>")) {
                termsOfUseButton->setText(defaultText + " <span style='color: red;'>!</span>");
            }
        } else {
            termsOfUseButton->setText(defaultText.remove(" <span style='color: red;'>!</span>"));
        }
    });


    connect(haveAccountButton, &QPushButton::clicked, this, &Registration::handle_have_an_account_btn);
    connect(termsOfUseButton, &QPushButton::clicked, this, &Registration::handle_terms_of_use_btn);
    connect(prevButton, &QPushButton::clicked, this, &Registration::handle_prev_btn);
    connect(registerButton, &QPushButton::clicked, this, &Registration::handle_reg_btn);

    save_texts();
}

/*bool Registration::checkFormValidity() {
    bool isFormValid = true;

    for(int i = 0; i < formLayout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = formLayout->itemAt(i, QFormLayout::FieldRole)->widget();

        if(!label && !field) {
            continue;
        }
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(field);

        if(lineEdit) {
            const QValidator* validator = lineEdit->validator();
            QString text = lineEdit->text();
            int pos = 0;
            bool isValid = validator ? (validator->validate(text, pos) == QValidator::Acceptable) : true;

            if(!isValid) {
                isFormValid = false;
                if(!label->text().contains("!")) {
                    label->setText(label->text()+ " <span style='color: red;'>!</span>");
                }
            } else {
                if(label->text().contains("!")) {
                    label->setText(label->text().remove(" <span style='color: red;'>!</span>"));
                }
            }
            continue;
        }

        QCheckBox* checkBox = qobject_cast<QCheckBox*>(field);
        if(checkBox) {
            if(!checkBox->isChecked()) {
                isFormValid = false;
                if(!label->text().contains("!")) {
                    label->setText(label->text()+ " <span style='color: red;'>!</span>");
                }
            } else {
                if(label->text().contains("!")) {
                    label->setText(label->text()+ " <span style='color: red;'>!</span>");
                }
            }
            continue;
        }
        QDateEdit* dateEdit = qobject_cast<QDateEdit*>(field);
        if(dateEdit) {
            QDate selectedDate = dateEdit->date();
            if(selectedDate.addYears(18) > QDate::currentDate()) {
                isFormValid = false;
                if(!label->text().contains("!")) {
                    label->setText(label->text()+ " <span style='color: red;'>!</span>");
                }
            } else {
                if(label->text().contains("!")) {
                    label->setText(label->text()+ " <span style='color: red;'>!</span>");
                }
            }
            continue;
        }
    }

    return isFormValid;
}*/

void Registration::handle_reg_btn()
{
    bool allValid = true;
    for(int i = 0; i < formLayout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
        if(!label) {
            continue;
        }
        if(!label->text().contains("!")) {
            allValid = false;
            break;
        }
        if (registerButton && !registerButton->isEnabled()) {
            registerButton->setEnabled(allValid);
        }

        if(allValid) {

            QUrl url("http://192.168.35.83:8000/register/");
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

            if(globalResponse != "") {
                client_login->postRequest(url, jsonData);
            }
            else {
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

        nicknameField->setPlaceholderText(tr("Enter your nickname"));
        passwordField->setPlaceholderText(tr("Enter your password"));
        confirmPasswordField->setPlaceholderText(tr("Reenter your password"));

        nameLabel->setText(tr("First name: "));
        surnameLabel->setText(tr("Last name (optional): "));
        nicknameLabel->setText(tr("Nickname: "));
        emailLabel->setText(tr("Email address: "));
        dateLabel->setText(tr("Date of birth: "));
        passwordLabel->setText(tr("Password: "));
        confirmPasswordLabel->setText(tr("Confirm password: "));

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
