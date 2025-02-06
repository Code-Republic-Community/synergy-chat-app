#include "login.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QJsonObject>
#include "httpclient.h"
#include <QJsonDocument>
#include "globals.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
{
    init();
}

void Login::init()
{
    setFixedSize(400, 700);

    client_login = new HttpClient();

    m_loginLabel = new QLabel(this);
    m_fontForLogin = m_loginLabel->font();
    m_fontForLogin.setPointSize(30);
    m_loginLabel->setFont(m_fontForLogin);
    m_loginLabel->setGeometry(150, 100, 150, 60);

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

    m_dontHaveAnAccount = new QLabel(this);
    m_dontHaveAnAccount->setGeometry(110, 460, 200, 150);

    m_register = new QLabel(this);
    m_register->setGeometry(110, 543, 60, 20);
    m_register->setStyleSheet("color: blue; text-decoration: underline;");
    m_register->setCursor(Qt::PointingHandCursor);

    //Next and Prev buttons
    m_nextAndPrev = new navigationPrevOrNext(this);
    m_nextAndPrev->setGeometry(175, 565, 400, 100);

    m_forget = new QLabel(this);
    m_forget->setGeometry(165, 530, 250, 150);
    m_forget->setStyleSheet("color: red;");
    m_forget->hide();

    forgetIcon();
    setLanguage();

    connect(m_rememberMe, &QCheckBox::toggled, this, &Login::rememberMe);
    connect(m_nextAndPrev, &navigationPrevOrNext::nextClicked, this, &Login::handleNextButtonClicked);
    connect(m_nextAndPrev, &navigationPrevOrNext::prevClicked, this, &Login::handlePrevButtonClicked);
    connect(client_login, &HttpClient::responseReceived, this, &Login::handleUserId);
}

void Login::setLanguage()
{
    m_loginLabel->setText(tr("Login"));

    m_underLoginText->setText(tr("Enter your username and password.\n"
                                 "If you don't have an account, register below."));

    m_usernameLabel->setText(tr("Username"));
    usernameLineEdit->setPlaceholderText(tr("Username"));

    m_passwordLabel->setText(tr("Password"));
    passwordLineEdit->setPlaceholderText(tr("Password"));

    m_rememberMe->setText(tr("Remember Me"));

    m_dontHaveAnAccount->setText(tr("Don't have an account"));
    m_register->setText("register");

    m_nextAndPrev->setLeftButton(tr("Prev"));
    m_nextAndPrev->setRightButton(tr("Next"));

    m_forget->setText(tr("Both must be complementary"));
}

void Login::forgetIcon()
{
    m_label1 = new QLabel(this);
    m_label2 = new QLabel(this);

    m_label1 = new QLabel("❌", this);
    m_label1->setStyleSheet("color: red;");
    m_label2 = new QLabel("❌", this);
    m_label2->setStyleSheet("color: red;");

    m_label1->setGeometry(197, 322, 10, 10);
    m_label2->setGeometry(192, 392, 10, 10);

    m_label1->hide();
    m_label2->hide();
}

void Login::saveTexts()
{
    m_usernameText = usernameLineEdit->text();
    m_passwordText = passwordLineEdit->text();
}

void Login::handleUserId(QByteArray responseData)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();

    if (jsonObject.contains("user_id")) {
        Globals::getInstance().setUserID(jsonObject["user_id"].toString());
        qDebug() << Globals::getInstance().getUserId();
        emit idreceived();
    } else {
        qDebug() << "User ID not found in response.";
    }
}

void Login::rememberMe(bool isClicked)
{
    qDebug() << "Checkbox toggled: " << isClicked;

    if(isClicked) {
        // Remember login and password logic
    } else {
        //  Do not remember
    }
}

void Login::mousePressEvent(QMouseEvent* event)
{
    if (m_register->underMouse()) {
        qDebug() << "Register link clicked!";
        emit register_signal();
    }
    if(event) {

    }
}
void Login::handleNextButtonClicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    if (username.isEmpty() && password.isEmpty()) {
        m_label1->show();
        m_label2->show();
        m_forget->show();
        QTimer::singleShot(3000, m_forget, &QLabel::hide);
        return;

    } else if(username.isEmpty()) {
        m_label1->show();
        m_label2->hide();
        m_forget->show();
        QTimer::singleShot(3000, m_forget, &QLabel::hide);
        return;

    } else if(password.isEmpty()) {
        m_label2->show();
        m_label1->hide();
        m_forget->show();
        QTimer::singleShot(3000, m_forget, &QLabel::hide);
        return;
    } else {
        m_label1->hide();
        m_label2->hide();

        saveTexts();

        QUrl url("https://synergy-iauu.onrender.com/login/");
        QJsonObject jsonData;
        jsonData["nickname"] = m_usernameText;
        jsonData["password"] = m_passwordText;
        qDebug() << jsonData.keys();
        qDebug() << jsonData.value("nickname");
        qDebug() << jsonData.value("password");

        client_login->postRequest(url, jsonData);
        emit next_btn_signal();
    }

    // client_login->getRequest(url);
    // client_login->postRequest(url, jsonData);
    // client_login->putRequest(url, jsonData);
    // client_login->deleteRequest(url);

    // QString search_tmp = "http://192.168.35.83:8000/search/";

    // QJsonObject jsonData2;
    // jsonData2["user_id"] = "ec569aee-05b2-42c6-8917-2448fae102d6";
    // jsonData2["search_string"] = "arxitekt0r";
    // search_tmp += jsonData2.value("user_id").toString() + "/" + jsonData2.value("search_string").toString();
    // QUrl url2(search_tmp);
    // qDebug() << jsonData2.keys();
    // qDebug() << jsonData2.value("user_id");
    // qDebug() << jsonData2.value("search_string");
    // client_login->getRequest(url2);
}

void Login::handlePrevButtonClicked()
{
    emit prev_btn_signal();
}

Login::~Login() {}
