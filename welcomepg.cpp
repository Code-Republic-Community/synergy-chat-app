#include "welcomepg.h"
#include "translator.h"
WelcomePg::WelcomePg(QWidget *parent)
    : QWidget(parent)
    , welcome_label(new QLabel(this))
    , text_tras(new QLabel(this))
{
    this->setFixedSize(400, 700);
    init();
    conecting();
}

void WelcomePg::init(){
    QWidget *central = new QWidget(this);
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

    languageComboBox = new QComboBox(this);
    languageComboBox->addItem(tr("English"), "en_US");
    languageComboBox->addItem(tr("Armenian"), "hy_AM");
    languageComboBox->addItem(tr("Russian"), "ru_RU");
    languageComboBox->setGeometry(20, 600, 100, 30);
    about = new QPushButton(this);
    about->setGeometry(280, 600, languageComboBox->width(), languageComboBox->height());
    about->setStyleSheet("font-size: 15px;");

    setLayout(layout);
    setLanguage();
}

void WelcomePg::conecting(){
    connect(signIn, &QPushButton::clicked, this, &WelcomePg::onSignInClicked);
    connect(signUp, &QPushButton::clicked, this, &WelcomePg::onSignUpClicked);
    connect(about, &QPushButton::clicked, this, [this]() { QMessageBox::information(this, "About", "A cross-platform desktop chat application built with Qt, offering real-time messaging and a clean, user-friendly interface.");});
    connect(languageComboBox, &QComboBox::currentIndexChanged, this, [this](int index){
        QString newLang = languageComboBox->itemData(index).toString();
        Translator::get().set(newLang);
        emit languageChanged();
    });

}

void WelcomePg::setLanguage(){
    welcome_label->setText(tr("Synergy"));
    text_tras->setText(tr("What is Lorem Ipsum?"));
    signUp->setText(tr("Sign up"));
    signIn->setText(tr("Sign in"));
    about->setText(tr("About"));

}

void WelcomePg::onSignInClicked(){
    emit signInClicked();
}

void WelcomePg::onSignUpClicked(){
    emit signUpClicked();
}

WelcomePg::~WelcomePg() {

}
