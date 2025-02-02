#include "welcomepg.h"

WelcomePg::WelcomePg(QWidget *parent)
    : QMainWindow(parent)
    , welcome_label(new QLabel(this))
    , text_tras(new QLabel(this))
{
    this->setFixedSize(400, 700);
    init();
    conecting();
}

void
WelcomePg::init(){
    QWidget *central = new QWidget(this);
    this->setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);
    welcome_label->setAlignment(Qt::AlignCenter);
    // welcome_label->setGeometry(0, 20, 200, 30);
    welcome_label->setStyleSheet("font-size: 30px; font-weight: bold; color: #FFFFFF;");
    //layout->setContentsMargins(0, 20, 0, 0);
    layout->addWidget(welcome_label, 0, Qt::AlignTop | Qt::AlignHCenter);


    text_tras->setStyleSheet("font-size: 15px; color: hsla(300, 100%, 40%, 0.4);");
    layout->addWidget(text_tras, 1, Qt::AlignTop | Qt::AlignHCenter);

    signUp = new QPushButton(this);
    signUp->setGeometry(100, 200, 200, 80);
    signUp->setStyleSheet("font-size: 20px; font-weight: bold;");

    signIn = new QPushButton(this);
    signIn->setGeometry(100, 300, 200, 80);
    signIn->setStyleSheet("font-size: 20px; font-weight: bold;");

    webPg = new QPushButton(this);
    webPg->setGeometry(20, 600, 80, 60);
    webPg->setStyleSheet("font-size: 15px;");

    about = new QPushButton(this);
    about->setGeometry(300, 600, 80, 60);
    about->setStyleSheet("font-size: 15px;");

    setLaguage();
}

void
WelcomePg::conecting(){
    connect(signIn, &QPushButton::clicked, this, &WelcomePg::onSignInClicked);
    connect(signUp, &QPushButton::clicked, this, &WelcomePg::onSignUpClicked);
    connect(about, &QPushButton::clicked, this, [this]() { QMessageBox::information(this, "About", "What is Lorem Ipsum?Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");});
    connect(webPg, &QPushButton::clicked, this,  [this]() { //hide();
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Web Page");
        dialog->resize(400, 700);
        dialog->show();});
}

void
WelcomePg::setLaguage(){
    welcome_label->setText(tr("Welcome to app's name"));
    text_tras->setText(tr("What is Lorem Ipsum?"));
    signUp->setText(tr("Sign up"));
    signIn->setText(tr("Sign in"));
    webPg->setText(tr("Web page"));
    about->setText(tr("About"));

}

void
WelcomePg::onSignInClicked(){
    emit signInClicked();
}

void
WelcomePg::onSignUpClicked(){
    emit signUpClicked();
}

WelcomePg::~WelcomePg() {
    delete welcome_label;
    delete text_tras;
    delete signUp;
    delete signIn;
    delete webPg;
    delete about;
}
