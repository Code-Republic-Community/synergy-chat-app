#include "registration.h"

#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QDate>

Registration::Registration(QWidget *parent)
    : QWidget(parent)

{
    centralWidget = new QWidget(this);
    this->setFixedSize(400, 700);
    this->setStyleSheet("background-color: lightblue;");

    mainlayout = new QVBoxLayout;


    toplabel = new QLabel("REGISTRATION");
    toptext = new QLabel("Dear user! Welcome to our page!");

    toplabel->setAlignment(Qt::AlignCenter);
    toplabel->setStyleSheet("margin-top: 10px; margin-bottom: 50px");

    toplayout = new QHBoxLayout;
    middlelayout = new QHBoxLayout;
    bottomlayout = new QHBoxLayout;

    toplayout->addWidget(toplabel);

    formlayout = new QFormLayout;

    nameField = new QLineEdit;
    nameField->setPlaceholderText("John");
    surnameField = new QLineEdit;
    surnameField->setPlaceholderText("Smidth");
    nicknameField = new QLineEdit;
    nicknameField->setPlaceholderText("Enter your nickname");
    emailField = new QLineEdit;
    emailField->setPlaceholderText("john.smidth@yahoo.com");
    passwordField = new QLineEdit;
    passwordField->setPlaceholderText("Enter your password");
    confirmPasswordField = new QLineEdit;
    confirmPasswordField->setPlaceholderText("Reenter your password");
    checkbox = new QCheckBox;

    checkbox->setStyleSheet("margin-top: 10px; margin-bottom: 20px;");

    passwordField->setEchoMode(QLineEdit::Password);
    confirmPasswordField->setEchoMode(QLineEdit::Password);

    dateEdit = new QDateEdit;
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setMaximumDate(QDate::currentDate().addYears(-18));


    formlayout->addRow("First name:", nameField);
    formlayout->addRow("Last name (optional):", surnameField);
    formlayout->addRow("Nickname:", nicknameField);
    formlayout->addRow("Email address:", emailField);
    formlayout->addRow("Date of birth:", dateEdit);

    formlayout->addRow("Password:", passwordField);
    formlayout->addRow("Confirm password:", confirmPasswordField);
    //formlayout->addItem(new QSpacerItem(0, 2));
    formlayout->addRow("Terms of use", checkbox);
    formlayout->setSpacing(20);
    formlayout->setContentsMargins(10, 10, 20, 10);

    formlayout->setLabelAlignment(Qt::AlignRight);

    middlelayout->addLayout(formlayout);

    //QMap <QString, QRegularExpression> fieldRegex;//

    /* QRegularExpression nameRegex("^[A-Z][a-zA-Z]{2,}$");
    QRegularExpression surnameRegex("^[A-Z][a-z]{1,}$");
    QRegularExpression nicknameRegex("^[a-zA-Z][a-zA-Z0-9_]{2,}$");
    QRegularExpression emailRegex("^[\\w\\.]+@[\\w\\.]+\\.[a-z]{2,}$");
    fieldRegex["Password:"] = QRegularExpression("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");
    fieldRegex["Confirm password:"] = QRegularExpression("^(?=(.*[A-Z]){2,})(?=(.*[a-z]){2,})(?=(.*\\d){2,})(?=(.*[!@#$%^&*(),.?\":{}|<>]){2,}).{8,}$");
    */






    bottomlabel = new QLabel("Already have an account?");
    loginbutton = new QPushButton("Login");
    prevbutton = new QPushButton("< Previous");
    registerbutton = new QPushButton("Register >");

    bottomlayout->addWidget(bottomlabel);
    bottomlayout->addWidget(loginbutton);
    bottomlayout->addWidget(prevbutton);
    bottomlayout->addWidget(registerbutton);

    /*bottomlabel->setStyleSheet("margin-top:: 10px; margin-bottom:: 200px;");
    loginbutton->setStyleSheet("margin-top:: 10px; margin-bottom:: 200px;");
    prevbutton->setStyleSheet("margin-top:: 10px; margin-bottom:: 100px;");
    registerbutton->setStyleSheet("margin-top:: 10px; margin-bottom:: 100px;"); */

    loginbutton->setStyleSheet("QPushButton { background-color: lightgrey; color: grey; border: 2px solid darkgrey; }");
    prevbutton->setStyleSheet("QPushButton { background-color: lightgrey; color: grey; border: 2px solid darkgrey; }");
    registerbutton->setStyleSheet("QPushButton { background-color: lightgrey; color: grey; border: 2px solid darkgrey; }");

    mainlayout->addLayout(toplayout);
    mainlayout->addLayout(middlelayout);
    mainlayout->addLayout(bottomlayout);

    // setCentralWidget(centralWidget);

    centralWidget->setLayout(mainlayout);

    /*connect(nameField, &QLineEdit::textChanged(), this, [nameRegex, nameField] {
    if(nameRegex.match(nameField->text()).hasMatch()) {
    nameField->setStyleSheet("border: 2px solid red;");
    } else
        nameField->setStyleSheet("");
    });
    connect(surnameField, &QLineEdit::textChanged(), this, &MainWindow::surnameChanged());
    connect(nicknameField, &QLineEdit::textChanged(), this, &MainWindow::nicknameChanged());
    connect(emailField, &QLineEdit::textChanged(), this, &MainWindow::emailChanged());
    connect(passwordField, &QLineEdit::textChanged(), this, &MainWindow::passwordChanged());
    connect(confirmPasswordField, &QLineEdit::textChanged(), this, &MainWindow::confirmpasswordChanged());

    connect(dateEdit, &QDateEdit::dateChanged(), this, &MainWindow::dobChanged());
    */

    connect(prevbutton, &QPushButton::clicked, this, &Registration::handle_prev_btn);
    connect(registerbutton, &QPushButton::clicked, this, &Registration::handle_reg_btn);

}


Registration::~Registration() {}

void Registration::handle_reg_btn()
{
    emit reg_btn_signal();
}

void Registration::handle_prev_btn()
{
    emit prev_btn_signal();
}

/*void MainWindow::nameChanged(const QString &newname) {


};

void MainWindow::surnameChanged(const QString &newsurname) {


};

void MainWindow::nicknameChanged(const QString &newnickname) {


};
void MainWindow::emailChanged(const QString &newemail) {


};

void MainWindow::dobChanged(const QString &newdate) {


};

void MainWindow::passwordChanged(const QString &newpassword) {


};

void MainWindow::confirmPasswordChanged(const QString &confirmnewpassword) {


};*/
