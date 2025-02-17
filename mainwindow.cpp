#include "mainwindow.h"
#include "loadingwidget.h"
#include <QDir>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    welcome_pg = new WelcomePg();
    login_pg = new Login();
    reg_pg = new Registration();
    main_pg = new MainPageWindow();
    chat_pg = new ChatWidget();
    profile_settings_pg = new MyProfile();
    settings_pg = new Settings();
    verification_pg = new Verification();
    other_profile_pg = new OtherProfile();
    loading_page = new LoadingWidget();

    staked_widget = new QStackedWidget();
    staked_widget->addWidget(welcome_pg);          // 0
    staked_widget->addWidget(login_pg);            // 1
    staked_widget->addWidget(reg_pg);              // 2
    staked_widget->addWidget(main_pg);             // 3
    staked_widget->addWidget(chat_pg);             // 4
    staked_widget->addWidget(profile_settings_pg); // 5
    staked_widget->addWidget(settings_pg);         // 6
    staked_widget->addWidget(verification_pg);     // 7
    staked_widget->addWidget(other_profile_pg);    // 8
    staked_widget->addWidget(loading_page);        // 9

    connect(welcome_pg, &WelcomePg::signInClicked, login_pg, &Login::loadCredentials);
    connect(welcome_pg, &WelcomePg::signInClicked, this, &MainWindow::goToSignIn);
    connect(welcome_pg, &WelcomePg::signUpClicked, this, &MainWindow::goToRegPg);

    connect(login_pg, &Login::idreceived, profile_settings_pg, &MyProfile::handleIdReceiving);
    connect(reg_pg, &Registration::idreceived, profile_settings_pg, &MyProfile::handleIdReceiving);
    connect(login_pg, &Login::idreceived, main_pg, &MainPageWindow::handleIdReceiving);
    connect(reg_pg, &Registration::idreceived, main_pg, &MainPageWindow::handleIdReceiving);

    connect(login_pg, &Login::prev_btn_signal, this, &MainWindow::goToWelcomePg);

    connect(login_pg, &Login::next_btn_signal, this, &MainWindow::goToMainPg);

    connect(reg_pg, &Registration::prev_btn_signal, this, &MainWindow::goToWelcomePg);

    connect(main_pg, &MainPageWindow::vchat_clicked_from_main_pg, this, &MainWindow::goToChatPg);
    connect(main_pg, &MainPageWindow::vchat_clicked_from_main_pg, chat_pg, &ChatWidget::handleDataFromMainPage);

    connect(main_pg,
            &MainPageWindow::profile_button_signal,
            this,
            &MainWindow::goToProfileSettingsPg);

    connect(profile_settings_pg, &MyProfile::gotoSettingsSignal, this, &MainWindow::goToSettings);

    connect(profile_settings_pg, &MyProfile::goBackSignal, this, &MainWindow::goToMainPg);

    connect(login_pg, &Login::register_signal, this, &MainWindow::goToRegPg);

    connect(reg_pg, &Registration::reg_btn_signal, this, &MainWindow::goToVerificationPg);

    connect(verification_pg, &Verification::prevClicked, this, &MainWindow::goToRegPg);

    connect(verification_pg, &Verification::nextClicked, this, &MainWindow::goToMainPg);

    connect(settings_pg, &Settings::goBackSignal, this, &MainWindow::goToProfileSettingsPg);

    connect(chat_pg, &ChatWidget::other_profile_signal, this, &MainWindow::goToOtherProfilePg);
    connect(chat_pg, &ChatWidget::other_profile_signal, other_profile_pg, &OtherProfile::handleDataFromChat);

    connect(reg_pg, &Registration::have_an_account_signal, this, &MainWindow::goToSignIn);

    connect(other_profile_pg, &OtherProfile::goBackSignal, this, [this]() {
        goToChatPg();
    });

    connect(chat_pg, &ChatWidget::go_back_signal, this, &MainWindow::goToMainPg);
    connect(profile_settings_pg,
            &MyProfile::logOutSiganl,
            main_pg,
            &MainPageWindow::handleContactReDonwnload);

    connect(settings_pg, &Settings::languageChanged, this, &MainWindow::change_language);

    connect(profile_settings_pg, &MyProfile::logOutSiganl, this, &MainWindow::goToSignIn);
    connect(profile_settings_pg,
            &MyProfile::logOutSiganl,
            main_pg,
            &MainPageWindow::clearDataOnLogout);

    connect(welcome_pg, &WelcomePg::languageChanged, this, &MainWindow::change_language);

    connect(reg_pg, &Registration::email_obt_signal, verification_pg, &Verification::handleEmail);

    connect(login_pg, &Login::startloading, this, &MainWindow::goto_loading_page);
    connect(login_pg, &Login::stoploading, this, &MainWindow::goToSignIn);

    connect(reg_pg, &Registration::startloading, this, &MainWindow::goto_loading_page);
    connect(reg_pg, &Registration::stoploading, this, &MainWindow::goToRegPg);

    connect(verification_pg, &Verification::startloading, this, &MainWindow::goto_loading_page);
    connect(verification_pg, &Verification::stoploading, this, &MainWindow::goToVerificationPg);

    openSavedAccount();

    this->setCentralWidget(staked_widget);
    this->setFixedSize(400, 700);
}

MainWindow::~MainWindow() {}

void MainWindow::openSavedAccount()
{
    goto_loading_page();
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
    if (file.exists()){
        goToSignIn();
        login_pg->loadCredentials();
    }
    else {
        goToWelcomePg();
        qDebug() << "Failed to open file for reading go to welocme page";
    }
}

void MainWindow::goToSignIn()
{
    staked_widget->setCurrentIndex(1);
    reg_pg->clear_fields();
    verification_pg->clear_fields();
}

void MainWindow::goToRegPg()
{
    staked_widget->setCurrentIndex(2);
    login_pg->clear_fields();
    verification_pg->clear_fields();
}

void MainWindow::goToWelcomePg()
{
    staked_widget->setCurrentIndex(0);
    reg_pg->clear_fields();
    verification_pg->clear_fields();
}

void MainWindow::goToMainPg()
{
    staked_widget->setCurrentIndex(3);
    reg_pg->clear_fields();
    verification_pg->clear_fields();
}

void MainWindow::goToChatPg()
{
    staked_widget->setCurrentIndex(4);
}

void MainWindow::goToProfileSettingsPg()
{
    staked_widget->setCurrentIndex(5);
}

void MainWindow::goToSettings()
{
    staked_widget->setCurrentIndex(6);
}

void MainWindow::goToVerificationPg()
{
    staked_widget->setCurrentIndex(7);
}

void MainWindow::goToOtherProfilePg()
{
    staked_widget->setCurrentIndex(8);
}

void MainWindow::goto_loading_page()
{
    staked_widget->setCurrentIndex(9);
}

void MainWindow::change_language()
{
    welcome_pg->setLanguage();
    login_pg->setLanguage();
    reg_pg->setLanguage();
    main_pg->setLanguage();
    chat_pg->setLanguage();
    profile_settings_pg->setLanguage();
    settings_pg->setLanguage();
    verification_pg->setLanguege();
    other_profile_pg->setLanguage();
}


