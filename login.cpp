#include "login.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

Login::Login(QWidget *parent)
    : QMainWindow(parent)
{
    init();
}

void Login::init()
{
    setFixedSize(400, 700);

    m_loginLabel = new QLabel("Login", this);
    m_fontForLogin = m_loginLabel->font();
    m_fontForLogin.setPointSize(30);
    m_loginLabel->setFont(m_fontForLogin);
    m_loginLabel->setGeometry(150, 100, 150, 60);

    m_underLoginText = new QLabel(
        "Welcome to the Login Page.\nEnter your username and password.\n"
        "If you don't have an account, you can register by clicking 'Register'.\n"
        "Your information is safe with us.",
        this
        );
    m_underLoginText->setWordWrap(true);
    m_underLoginText->setGeometry(90, 170, 300, 100);

    m_usernameLabel = new QLabel("Username", this);
    m_usernameLabel->setGeometry(125, 310, 150, 30);

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Username");
    usernameLineEdit->setGeometry(125, 340, 150, 30);

    m_passwordLabel = new QLabel("Password", this);
    m_passwordLabel->setGeometry(125, 380, 150, 30);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setGeometry(125, 410, 150, 30);

    m_rememberMe = new QCheckBox("Remember Me", this);
    m_rememberMe->setGeometry(125, 440, 150, 30);

    m_dontHaveAnAccount = new QLabel("Don't have an account", this);
    m_dontHaveAnAccount->setGeometry(110, 460, 200, 150);

    m_register = new QLabel("register", this);
    m_register->setGeometry(110, 475, 200, 150);
    m_register->setStyleSheet("color: blue; text-decoration: underline;");
    m_register->setCursor(Qt::PointingHandCursor);

    //Next and Prev buttons
    m_nextAndPrev = new navigationPrevOrNext(this);
    m_nextAndPrev->setGeometry(175, 565, 400, 100);

    m_forget = new QLabel("Both must be complementary", this);
    m_forget->setGeometry(165, 530, 250, 150);
    m_forget->setStyleSheet("color: red;");
    m_forget->hide();

    forgetIcon();

    connect(m_rememberMe, &QCheckBox::toggled, this, &Login::rememberMe);
    connect(m_nextAndPrev, &navigationPrevOrNext::nextClicked, this, &Login::handleNextButtonClicked);
    connect(m_nextAndPrev, &navigationPrevOrNext::prevClicked, this, &Login::handlePrevButtonClicked);
}

void Login::forgetIcon()
{
    m_imageLabel1 = new QLabel(this);
    m_imageLabel2 = new QLabel(this);

    QPixmap pixmap("/home/gegham/AllQtFolders/LoginPage/delete.png");

    if (pixmap.isNull()) {
        qDebug() << "Failed to load image: delete.png";
    } else {
        QPixmap scaledPixmap = pixmap.scaled(10, 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_imageLabel1->setPixmap(scaledPixmap);
        m_imageLabel1->setGeometry(197, 322, 10, 10);
        m_imageLabel2->setPixmap(scaledPixmap);
        m_imageLabel2->setGeometry(192, 392, 10, 10);
    }
    m_imageLabel1->hide();
    m_imageLabel2->hide();

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

void Login::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_register->underMouse()) {
        qDebug() << "Register link clicked!";
    }
    if(event) {

    }
}

#include <QMessageBox>

void Login::handleNextButtonClicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    if (username.isEmpty() && password.isEmpty()) {
        m_imageLabel1->show();
        m_imageLabel2->show();
        m_forget->show();
        QTimer::singleShot(3000, m_forget, &QLabel::hide);
        // return;

    } else if(username.isEmpty()) {
        m_imageLabel1->show();
        m_imageLabel2->hide();
        m_forget->show();
        QTimer::singleShot(3000, m_forget, &QLabel::hide);
        // return;

    } else if(password.isEmpty()) {
        m_imageLabel2->show();
        m_imageLabel1->hide();
        m_forget->show();
        QTimer::singleShot(3000, m_forget, &QLabel::hide);
        // return;
    } else {
        m_imageLabel1->hide();
        m_imageLabel2->hide();
        //Next button logic
    }

    emit next_btn_signal();
}

void Login::handlePrevButtonClicked()
{
    emit prev_btn_signal();
}

Login::~Login() {}
