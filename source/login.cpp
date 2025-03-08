#include "login.h"
#include "globals.h"
#include "httpclient.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTextStream>
#include <QStyle>

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    init();
    setLanguage();
}

void Login::saveCredentials(const QString &userId, const QString &username, const QString &password)
{
    qDebug() << "Save Credentials";

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                   + "/Synergy/credentials.txt";
    QString folderPath = QFileInfo(path).absolutePath(); // Get the folder path

    QDir dir;
    if (!dir.exists(folderPath)) {
        if (dir.mkpath(folderPath)) {
            qDebug() << "Created folder:" << folderPath;
        } else {
            qDebug() << "Failed to create folder";
            return;
        }
    }

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << userId << "\n" << username << "\n" << password;
        qDebug() << "Saving Credentials" << userId << "\n" << username << "\n" << password;
        file.close();
        qDebug() << "Credentials Saved Successfully";
    } else {
        qDebug() << "Failed to open file for writing";
    }
}

void Login::loadCredentials()
{
    emit startloading();
    qDebug() << "Load Credentials";

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                   + "/Synergy/credentials.txt";
    QString folderPath = QFileInfo(path).absolutePath();

    QDir dir;
    if (!dir.exists(folderPath)) {
        if (dir.mkpath(folderPath)) {
            qDebug() << "Created folder:" << folderPath;
        } else {
            qDebug() << "Failed to create folder";
            return;
        }
    }


    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString userId = in.readLine();
        Globals::getInstance().setUserID(userId);
        QString username = in.readLine();
        usernameLineEdit->setText(username);
        QString password = in.readLine();
        passwordLineEdit->setText(password);
        file.close();

        qDebug() << "From File User ID = " << userId;
        qDebug() << "From File Username = " << username;
        qDebug() << "From File Password = " << password;
        remember = true;
        m_rememberMe->setCheckState(Qt::Checked);
        emit m_nextAndPrev->nextClicked();
    }
    else {
        qDebug() << "Failed to open file for reading";
        emit stoploading();
    }
}

void Login::clearCredentials()
{
    qDebug() << "Clear Credentials";

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                   + "/Synergy/credentials.txt";
    QFile file(path);

    QString folderPath = QFileInfo(path).absolutePath();
    QDir dir;
    if (!dir.exists(folderPath)) {
        qDebug() << "Folder does not exist, no need to remove file.";
        return;
    }

    if (file.exists()) {
        if (file.remove()) {
            qDebug() << "Credentials file removed successfully.";
        } else {
            qDebug() << "Failed to remove credentials file.";
        }
    } else {
        qDebug() << "Credentials file does not exist.";
    }
}

void Login::init()
{
    setFixedSize(400, 700);

    remember = false;
    client_login = new HttpClient();
    overlay = new LoadingOverlay(this);

    m_loginLabel = new QLabel(this);
    m_fontForLogin = m_loginLabel->font();
    m_fontForLogin.setPointSize(30);
    m_loginLabel->setFont(m_fontForLogin);
    m_loginLabel->setGeometry(150, 80, 150, 60);

    m_underLoginText = new QLabel(this);
    m_underLoginText->setWordWrap(true);
    m_underLoginText->setAlignment(Qt::AlignCenter);
    m_underLoginText->setGeometry(50, 150, 300, 60);

    m_usernameLabel = new QLabel(this);
    m_usernameLabel->setGeometry(125, 310, 150, 30);

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setGeometry(125, 340, 150, 30);

    m_passwordLabel = new QLabel(this);
    m_passwordLabel->setGeometry(125, 380, 150, 30);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setGeometry(125, 410, 150, 30);

    m_rememberMe = new QCheckBox(this);
    m_rememberMe->setGeometry(125, 440, 150, 30);
    m_rememberMe->setObjectName("m_rememberMe");

    m_dontHaveAnAccount = new QLabel(this);
    m_dontHaveAnAccount->setGeometry(125, 480, 200, 20);


    m_register = new QLabel(this);
    m_register->setGeometry(125, 500, 120, 20);
    m_register->setObjectName("m_register");
    m_register->setCursor(Qt::PointingHandCursor);

    m_nextAndPrev = new navigationPrevOrNext(this);
    m_nextAndPrev->setGeometry(95, 630, 210, 60);

    setLanguage();

    connect(m_rememberMe, &QCheckBox::toggled, this, &Login::rememberMe);
    connect(m_nextAndPrev,
            &navigationPrevOrNext::nextClicked,
            this,
            &Login::handleNextButtonClicked);
    connect(m_nextAndPrev,
            &navigationPrevOrNext::prevClicked,
            this,
            &Login::handlePrevButtonClicked);
    connect(client_login, &HttpClient::responseReceived, this, &Login::handleUserId);
}

void Login::setLanguage()
{
    m_loginLabel->setText(tr("Login"));

    m_underLoginText->setText(tr("Enter your username and password. If you don't have an account, register below."));

    m_usernameLabel->setText(tr("Username"));
    usernameLineEdit->setPlaceholderText(tr("Username"));

    m_passwordLabel->setText(tr("Password"));
    passwordLineEdit->setPlaceholderText(tr("Password"));

    m_rememberMe->setText(tr("Remember Me"));

    m_dontHaveAnAccount->setText(tr("Don't have an account ?"));
    m_register->setText(tr("Register"));

    m_nextAndPrev->setLeftButton(tr("Back"));
    m_nextAndPrev->setRightButton(tr("Next"));

}

void Login::clear_fields()
{
    m_rememberMe->setCheckState(Qt::Unchecked);
    usernameLineEdit->setText("");
    passwordLineEdit->setText("");
    usernameLineEdit->setProperty("error", false);
    passwordLineEdit->setProperty("error", false);
    usernameLineEdit->style()->unpolish(usernameLineEdit);
    usernameLineEdit->style()->polish(usernameLineEdit);
    passwordLineEdit->style()->unpolish(passwordLineEdit);
    passwordLineEdit->style()->polish(passwordLineEdit);
}

void Login::saveTexts()
{
    m_usernameText = usernameLineEdit->text();
    m_passwordText = passwordLineEdit->text();
}

void Login::handleUserId(QByteArray responseData)
{
    emit startloading();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();

    if (jsonObject.contains("user_id")) {
        Globals::getInstance().setUserID(jsonObject["user_id"].toString());
        qDebug() << Globals::getInstance().getUserId();
        if (remember) {
            saveCredentials(Globals::getInstance().getUserId(),
                            usernameLineEdit->text(),
                            passwordLineEdit->text());
        } else {
            clearCredentials();
        }
        emit stoploading();
        emit idreceived();
        emit next_btn_signal();
    } else {
        emit stoploading();
        qDebug() << "User ID not found in response.";
        usernameLineEdit->setProperty("error", true);
        passwordLineEdit->setProperty("error", true);
        usernameLineEdit->style()->unpolish(usernameLineEdit);
        usernameLineEdit->style()->polish(usernameLineEdit);
        passwordLineEdit->style()->unpolish(passwordLineEdit);
        passwordLineEdit->style()->polish(passwordLineEdit);
    }
}

void Login::rememberMe(bool isClicked)
{
    qDebug() << "Checkbox toggled: " << isClicked;
    if (isClicked) {
        remember = true;
    } else {
        remember = false;
        clearCredentials();
    }
}

void Login::mousePressEvent(QMouseEvent *event)
{
    if (m_register->underMouse()) {
        qDebug() << "Register link clicked!";
        emit register_signal();
    }
    if (event) {
    }

}
void Login::handleNextButtonClicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    if (username.isEmpty() && password.isEmpty()) {
        usernameLineEdit->setProperty("error", true);
        passwordLineEdit->setProperty("error", true);
        usernameLineEdit->style()->unpolish(usernameLineEdit);
        usernameLineEdit->style()->polish(usernameLineEdit);
        passwordLineEdit->style()->unpolish(passwordLineEdit);
        passwordLineEdit->style()->polish(passwordLineEdit);
        return;
    } else if (username.isEmpty()) {
        usernameLineEdit->setProperty("error", true);
        passwordLineEdit->setProperty("error", false);
        usernameLineEdit->style()->unpolish(usernameLineEdit);
        usernameLineEdit->style()->polish(usernameLineEdit);
        passwordLineEdit->style()->unpolish(passwordLineEdit);
        passwordLineEdit->style()->polish(passwordLineEdit);
        return;
    } else if (password.isEmpty()) {
        usernameLineEdit->setProperty("error", false);
        passwordLineEdit->setProperty("error", true);
        usernameLineEdit->style()->unpolish(usernameLineEdit);
        usernameLineEdit->style()->polish(usernameLineEdit);
        passwordLineEdit->style()->unpolish(passwordLineEdit);
        passwordLineEdit->style()->polish(passwordLineEdit);
        return;
    } else {
        emit startloading();
        usernameLineEdit->setProperty("error", false);
        passwordLineEdit->setProperty("error", false);
        usernameLineEdit->style()->unpolish(usernameLineEdit);
        usernameLineEdit->style()->polish(usernameLineEdit);
        passwordLineEdit->style()->unpolish(passwordLineEdit);
        passwordLineEdit->style()->polish(passwordLineEdit);
        saveTexts();
        QUrl url(Globals::getInstance().getSERVER_LINK() + "login/");
        QJsonObject jsonData;
        jsonData["nickname"] = m_usernameText;
        jsonData["password"] = m_passwordText;
        qDebug() << jsonData.keys();
        qDebug() << jsonData.value("nickname");
        qDebug() << jsonData.value("password");

        client_login->postRequest(url, jsonData);
    }
}

void Login::handlePrevButtonClicked()
{
    emit stoploading();
    qDebug() << "Back button clicked, emitting prev_btn_signal";
    emit prev_btn_signal();
}

Login::~Login() {}
