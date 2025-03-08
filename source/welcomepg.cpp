#include "welcomepg.h"
#include "translator.h"

WelcomePg::WelcomePg(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    init();
    conecting();
    setLanguage();
}

void WelcomePg::init()
{
    theme = "Light";
    welcome_label = new QLabel(this);
    welcome_label->setText("Synergy");
    welcome_label->setObjectName("welcome_label");
    welcome_label->setStyleSheet("font-size: 30px; font-weight: bold;");
    welcome_label->adjustSize();
    QSize size = welcome_label->size();
    welcome_label->setGeometry((400 - size.width()) / 2, 20, size.width(), size.height());

    thememode = new QPushButton(this);
    thememode->setFixedSize(30, 30);
    thememode->setIcon(QIcon(":/pngs/resources/darkmode.ico"));
    thememode->move(400 - 50, 20);


    signUp = new QPushButton(this);
    signUp->setGeometry(100, 200, 200, 80);

    signIn = new QPushButton(this);
    signIn->setGeometry(100, 300, 200, 80);

    languageComboBox = new QComboBox(this);
    languageComboBox->addItem(englishText, "en_US");
    languageComboBox->addItem(armenianText, "hy_AM");
    languageComboBox->addItem(russianText, "ru_RU");

    languageComboBox->setGeometry(20, 600, 140, 30);
    about = new QPushButton(this);
    about->setGeometry(240, 600, languageComboBox->width(), languageComboBox->height());
}

void WelcomePg::conecting()
{
    connect(signIn, &QPushButton::clicked, this, &WelcomePg::onSignInClicked);
    connect(signUp, &QPushButton::clicked, this, &WelcomePg::onSignUpClicked);
    connect(about, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(
            this, tr("About"), tr("A cross-platform desktop chat application built with Qt, offering real-time messaging and a clean, user-friendly interface."));
    });
    connect(languageComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        QString newLang = languageComboBox->itemData(index).toString();
        Translator::get().set(newLang);
        emit languageChanged();
    });
    connect(thememode, &QPushButton::clicked, this, [this](){
        if(theme == "Light")
        {
            thememode->setIcon(QIcon(":/pngs/resources/lightmode.ico"));
            theme = "Dark";
            emit theme_changed(theme);
        }
        else if (theme == "Dark")
        {
            thememode->setIcon(QIcon(":/pngs/resources/darkmode.ico"));
            theme = "Light";
            emit theme_changed(theme);
        }
    });
}

void WelcomePg::setLanguage()
{
    englishText = tr("English");
    armenianText = tr("Armenian");
    russianText = tr("Russian");
    languageComboBox->setItemText(0, englishText);
    languageComboBox->setItemText(1, armenianText);
    languageComboBox->setItemText(2, russianText);
    signUp->setText(tr("Sign up"));
    signIn->setText(tr("Sign in"));
    about->setText(tr("About"));
}

void WelcomePg::onSignInClicked()
{
    emit signInClicked();
}

void WelcomePg::onSignUpClicked()
{
    emit signUpClicked();
}

void WelcomePg::handle_theme_changed_from_settings(QString mode)
{
    theme = mode;
    if(theme == "Light")
    {
        thememode->setIcon(QIcon(":/pngs/resources/darkmode.ico"));
    }
    else if (theme == "Dark")
    {
        thememode->setIcon(QIcon(":/pngs/resources/lightmode.ico"));
    }
}

WelcomePg::~WelcomePg() {}
