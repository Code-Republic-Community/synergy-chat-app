// #include "registration.h"

// #include <QRegularExpressionValidator>
// #include <QRegularExpression>
// #include <QMessageBox>
// #include <QDate>

// Registration::Registration(QWidget *parent)
//     : QWidget(parent)

// {
//     this->setFixedSize(400, 700);
//     //init();

//     //}
//     //void Registration::init {
//     centralWidget = new QWidget(this);

//     this->setStyleSheet("background-color: grey;");

//     mainlayout = new QVBoxLayout;


//     toplabel = new QLabel("Registration");
//     toplabel->setText("<h1>Registration</h1>");
//     toplabel->move(150, 200);
//     toplabel->setAlignment(Qt::AlignCenter);

//     smalltext = new QLabel("<p style='font-size: 10px;'>A Binary Search Tree (or BST) is a data structure used in computer science for organizing and storing data in a sorted manner. This hierarchical structure allows for efficient searching, insertion, and deletion operations on the data stored in the tree.</p>");
//     smalltext->setWordWrap(true);
//     smalltext->setAlignment(Qt::AlignHCenter);
//     smalltext->adjustSize();

//     toplayout = new QVBoxLayout;
//     middlelayout = new QHBoxLayout;
//     bottomlayout = new QHBoxLayout;

//     toplayout->addWidget(toplabel, 0, Qt::AlignTop | Qt::AlignHCenter);
//     toplayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
//     toplayout->addWidget(smalltext, 0, Qt::AlignTop | Qt::AlignHCenter);

//     formlayout = new QFormLayout;

//     QRegularExpression nameRegex("^[A-Z][a-zA-Z]{2,}$");
//     QRegularExpression surnameRegex("^[A-Z][a-z]{1,}$");
//     QRegularExpression nicknameRegex("^[a-zA-Z][a-zA-Z0-9_]{2,}$");
//     QRegularExpression emailRegex("^[\\w\\.]+@[\\w\\.]+\\.[a-z]{2,}$");
//     QRegularExpression passwordRegex("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");
//     QRegularExpression confirmPasswordRegex("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");


//     nameField = new QLineEdit;
//     nameField->setPlaceholderText("John");
//     nameField->setValidator(new QRegularExpressionValidator(nameRegex, this));

//     surnameField = new QLineEdit;
//     surnameField->setPlaceholderText("Smidth");
//     surnameField->setValidator(new QRegularExpressionValidator(surnameRegex, this));
//     nicknameField = new QLineEdit;
//     nicknameField->setPlaceholderText("Enter your nickname");
//     nicknameField->setValidator(new QRegularExpressionValidator(nicknameRegex, this));
//     emailField = new QLineEdit;
//     emailField->setPlaceholderText("john.smidth@yahoo.com");
//     emailField->setValidator(new QRegularExpressionValidator(emailRegex, this));
//     passwordField = new QLineEdit;
//     passwordField->setPlaceholderText("Enter your password");
//     passwordField->setValidator(new QRegularExpressionValidator(passwordRegex, this));
//     confirmPasswordField = new QLineEdit;
//     confirmPasswordField->setPlaceholderText("Reenter your password");
//     confirmPasswordField->setValidator(new QRegularExpressionValidator(confirmPasswordRegex, this));

//     passwordField->setEchoMode(QLineEdit::Password);
//     confirmPasswordField->setEchoMode(QLineEdit::Password);

//     checkbox = new QCheckBox;
//     checkbox->setStyleSheet("margin-top: 0px; margin-bottom: 0px;");
//     checkbox->setStyleSheet("QCheckBox::indicator { background-color: white; }");

//     checkbox->setStyleSheet("margin-top: 0px; margin-bottom: 0px;");


//     termsOfUseButton = new QPushButton("Terms of use");
//     //termsOfUseButton->setStyleSheet("QPushButton { background-color: transparent; color: black; border: none; text-decoration: underline; } ");
//     termsOfUseButton->setStyleSheet("QPushButton { background-color: transparent; color: blue; border: none; text-decoration: underline; } ");
//     termsOfUseButton->setFixedHeight(checkbox->sizeHint().height());
//     QFont tfont = termsOfUseButton->font();
//     tfont.setPointSize(10);
//     termsOfUseButton->setFont(tfont);



//     dateEdit = new QDateEdit;
//     dateEdit->setCalendarPopup(true);
//     dateEdit->setDisplayFormat("dd/MM/yyyy");
//     dateEdit->setMaximumDate(QDate::currentDate().addYears(-18));


//     formlayout->addRow("First name:", nameField);
//     nameField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");


//     formlayout->addRow("Last name (optional):", surnameField);
//     surnameField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

//     formlayout->addRow("Nickname:", nicknameField);
//     nicknameField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

//     formlayout->addRow("Email address:", emailField);
//     emailField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

//     formlayout->addRow("Date of birth:", dateEdit);
//     dateEdit->setStyleSheet("QDateEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

//     formlayout->addRow("Password:", passwordField);
//     passwordField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");

//     formlayout->addRow("Confirm password:", confirmPasswordField);

//     confirmPasswordField->setStyleSheet("QLineEdit { background-color: lightgrey; width: 110; hеight: 30; border: 2 px solid grey; border-radius: 6px; }");



//     formlayout->addItem(new QSpacerItem(0, 20));
//     formlayout->addRow(termsOfUseButton, checkbox);
//     formlayout->setSpacing(20);
//     formlayout->setContentsMargins(10, 10, 20, 10);
//     formlayout->setLabelAlignment(Qt::AlignLeft);


//     middlelayout->addLayout(formlayout);


//     haveAccountButton = new QPushButton("Already have an account?");
//     //haveAccountButton->setStyleSheet("QPushButton { background-color: transparent; color: black; border: none; text-decoration: underline; } QPushButton:hover { color: green; }");
//     QFont font = haveAccountButton->font();
//     font.setPointSize(10);
//     haveAccountButton->setFont(font);
//     haveAccountButton->setStyleSheet("background-color: transparent; color: blue; border: none; text-decoration: underline; ");

//     QHBoxLayout* rowlayout = new QHBoxLayout;
//     rowlayout->addWidget(haveAccountButton);
//     rowlayout->addStretch();
//     formlayout->addRow(rowlayout);
//     formlayout->addItem(new QSpacerItem(0, 30));

//     //loginButton = new QPushButton("Login");
//     //loginButton->setStyleSheet("QPushButton { background-color: grey; color: black; width: 100px; height: 40px; border: 2px solid darkgrey; border-radius: 8px; }");
//     //formlayout->addWidget(loginButton);

//     prevButton = new QPushButton("< Previous");
//     prevButton->setStyleSheet("QPushButton { background-color: grey; color: black; width: 100px; height: 40px; border: 2px solid darkgrey; border-radius: 8px;}");

//     registerButton = new QPushButton("Register >");
//     registerButton->setEnabled(true);
//     registerButton->setStyleSheet("QPushButton { background-color: green; color: black; width: 100px; height: 40px; border: 2px solid green; border-radius: 8px;}");

//     bottomlayout->addWidget(prevButton);
//     bottomlayout->addWidget(registerButton);

//     mainlayout->addLayout(toplayout);
//     mainlayout->addLayout(middlelayout);
//     mainlayout->addLayout(bottomlayout);

//     // setCentralWidget(centralWidget);

//     centralWidget->setLayout(mainlayout);


//     connect(haveAccountButton, &QPushButton::clicked, this, &Registration::handle_have_an_account_btn);
//     connect(termsOfUseButton, &QPushButton::clicked, this, &Registration::handle_terms_of_use_btn);

//     connect(prevButton, &QPushButton::clicked, this, &Registration::handle_prev_btn);
//     connect(registerButton, &QPushButton::clicked, this, &Registration::handle_reg_btn);


// }

// void Registration::handle_reg_btn()
// {
//     if(!checkFormValidity()){
//         return;
//     }

//     registerButton->setEnabled(true);
//     emit reg_btn_signal();
// }

// void Registration::handle_prev_btn()
// {
//     emit prev_btn_signal();
// }

// void Registration::handle_have_an_account_btn()
// {
//     emit have_an_account_signal();
// }

// void Registration::handle_terms_of_use_btn()
// {
//     emit terms_of_use_signal();
// }


// bool Registration::checkFormValidity() {
//     bool isFormValid = true;

//     for(int i = 0; i < formlayout->rowCount(); ++i) {
//         QLabel* label = qobject_cast<QLabel*>(formlayout->itemAt(i, QFormLayout::LabelRole)->widget());
//         QWidget* field = formlayout->itemAt(i, QFormLayout::FieldRole)->widget();

//         if(!label && !field) {
//             continue;
//         }
//         QLineEdit* lineEdit = qobject_cast<QLineEdit*>(field);

//         if(lineEdit) {
//             const QValidator* validator = lineEdit->validator();
//             QString text = lineEdit->text();
//             int pos = 0;
//             bool isValid = validator ? (validator->validate(text, pos) == QValidator::Acceptable) : true;

//             if(!isValid) {
//                 isFormValid = false;
//                 if(!label->text().contains("!")) {
//                     label->setText(label->text()+ " <span style='color: red;'>!</span>");
//                 }
//             } else {
//                 if(label->text().contains("!")) {
//                     label->setText(label->text().remove(" <span style='color: red;'>!</span>"));
//                 }
//             }
//             continue;
//         }

//         QCheckBox* checkbox = qobject_cast<QCheckBox*>(field);
//         if(checkbox) {
//             if(!checkbox->isChecked()) {
//                 isFormValid = false;
//                 if(!label->text().contains("!")) {
//                     label->setText(label->text()+ " <span style='color: red;'>!</span>");
//                 }
//             } else {
//                 if(label->text().contains("!")) {
//                     label->setText(label->text()+ " <span style='color: red;'>!</span>");
//                 }
//             }
//             continue;
//         }
//         QDateEdit* dateEdit = qobject_cast<QDateEdit*>(field);
//         if(dateEdit) {
//             QDate selectedDate = dateEdit->date();
//             if(selectedDate.addYears(18) > QDate::currentDate()) {
//                 isFormValid = false;
//                 if(!label->text().contains("!")) {
//                     label->setText(label->text()+ " <span style='color: red;'>!</span>");
//                 }
//             } else {
//                 if(label->text().contains("!")) {
//                     label->setText(label->text()+ " <span style='color: red;'>!</span>");
//                 }
//             }
//             continue;
//         }
//     }

//     return isFormValid;
// }



// Registration::~Registration() {}


#include "registration.h"

#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDate>

Registration::Registration(QWidget *parent)
    : QWidget(parent)

{
    this->setFixedSize(400, 700);
    //init();

    //}
    //void Registration::init {
    centralWidget = new QWidget(this);

    this->setStyleSheet("background-color: grey;");

    mainLayout = new QVBoxLayout;

    topLabel = new QLabel;
    topLabel->setText("<h1>Registration</h1>");
    topLabel->move(150, 200);
    topLabel->setAlignment(Qt::AlignCenter);

    smallText = new QLabel("<p style='font-size: 10px;'>A Binary Search Tree (or BST) is a data structure used in computer science for organizing and storing data in a sorted manner. This hierarchical structure allows for efficient searching, insertion, and deletion operations on the data stored in the tree.</p>");
    smallText->setWordWrap(true);
    smallText->setAlignment(Qt::AlignHCenter);
    smallText->adjustSize();

    topLayout = new QVBoxLayout;
    middleLayout = new QHBoxLayout;
    bottomLayout = new QHBoxLayout;

    topLayout->addWidget(topLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    topLayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    topLayout->addWidget(smallText, 0, Qt::AlignTop | Qt::AlignHCenter);

    formLayout = new QFormLayout;

    QRegularExpression nameRegex("^[A-Z][a-zA-Z]{2,}$");
    QRegularExpression surnameRegex("^[A-Z][a-z]{1,}$");
    QRegularExpression nicknameRegex("^[a-zA-Z][a-zA-Z0-9_]{2,}$");
    QRegularExpression emailRegex("^[\\w\\.]+@[\\w\\.]+\\.[a-z]{2,}$");
    QRegularExpression passwordRegex("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");
    QRegularExpression confirmPasswordRegex("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");

    nameField = new QLineEdit;
    nameField->setPlaceholderText("John");
    nameField->setValidator(new QRegularExpressionValidator(nameRegex, this));

    surnameField = new QLineEdit;
    surnameField->setPlaceholderText("Smidth");
    surnameField->setValidator(new QRegularExpressionValidator(surnameRegex, this));
    nicknameField = new QLineEdit;
    nicknameField->setPlaceholderText("Enter your nickname");
    nicknameField->setValidator(new QRegularExpressionValidator(nicknameRegex, this));
    emailField = new QLineEdit;
    emailField->setPlaceholderText("john.smidth@yahoo.com");
    emailField->setValidator(new QRegularExpressionValidator(emailRegex, this));
    passwordField = new QLineEdit;
    passwordField->setPlaceholderText("Enter your password");
    passwordField->setValidator(new QRegularExpressionValidator(passwordRegex, this));
    confirmPasswordField = new QLineEdit;
    confirmPasswordField->setPlaceholderText("Reenter your password");
    confirmPasswordField->setValidator(new QRegularExpressionValidator(confirmPasswordRegex, this));

    passwordField->setEchoMode(QLineEdit::Password);
    confirmPasswordField->setEchoMode(QLineEdit::Password);

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

    // setCentralWidget(centralWidget);

    centralWidget->setLayout(mainLayout);

    connect(haveAccountButton, &QPushButton::clicked, this, &Registration::handle_have_an_account_btn);
    connect(termsOfUseButton, &QPushButton::clicked, this, &Registration::handle_terms_of_use_btn);

    connect(prevButton, &QPushButton::clicked, this, &Registration::handle_prev_btn);
    connect(registerButton, &QPushButton::clicked, this, &Registration::handle_reg_btn);

}

bool Registration::checkFormValidity() {
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
}

void Registration::handle_reg_btn()
{
    if(!checkFormValidity()){
        return;
    }

    registerButton->setEnabled(true);
    emit reg_btn_signal();
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


Registration::~Registration() {}

