#include "mainwindow.h"

extern QByteArray globalId;
// extern QByteArray globalRespsone;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    welcome_pg = new WelcomePg();
    login_pg = new Login();
    reg_pg = new Registration();
    main_pg = new MainPageWindow();
    chat_pg = new ChatWidget("TMP");
    profile_settings_pg = new MyProfile();
    settings_pg = new Settings();
    verification_pg = new Verification();
    other_profile_pg = new OtherProfile();

    staked_widget = new QStackedWidget();
    staked_widget->addWidget(welcome_pg); // 0
    staked_widget->addWidget(login_pg); // 1
    staked_widget->addWidget(reg_pg); // 2
    staked_widget->addWidget(main_pg); // 3
    staked_widget->addWidget(chat_pg); // 4
    staked_widget->addWidget(profile_settings_pg); // 5
    staked_widget->addWidget(settings_pg); // 6
    staked_widget->addWidget(verification_pg); // 7
    staked_widget->addWidget(other_profile_pg); // 8


    connect(welcome_pg, &WelcomePg::signInClicked, this, &MainWindow::goToSignIn);
    connect(welcome_pg, &WelcomePg::signUpClicked, this, &MainWindow::goToRegPg);

    connect(login_pg, &Login::prev_btn_signal, this, &MainWindow::goToWelcomePg);

    // if (globalRepsone == "") {
        connect(login_pg, &Login::next_btn_signal, this, &MainWindow::goToMainPg);
    // }

    connect(reg_pg, &Registration::prev_btn_signal, this, &MainWindow::goToWelcomePg);

    connect(main_pg, &MainPageWindow::vchat_clicked_from_main_pg, this, &MainWindow::goToChatPg);

    connect(main_pg, &MainPageWindow::profile_button_signal, this, &MainWindow::goToProfileSettingsPg);

    connect(profile_settings_pg, &MyProfile::gotoSettingsSignal, this, &MainWindow::goToSettings);

    connect(profile_settings_pg, &MyProfile::goBackSignal, this, &MainWindow::goToMainPg);

    connect(login_pg, &Login::register_signal, this, &MainWindow::goToRegPg);

    connect(reg_pg, &Registration::reg_btn_signal, this, &MainWindow::goToVerificationPg);

    connect(verification_pg, &Verification::prevClicked, this, &MainWindow::goToRegPg); // comment

    connect(verification_pg, &Verification::nextClicked, this, &MainWindow::goToMainPg);

    connect(settings_pg, &Settings::goBackSignal, this, &MainWindow::goToProfileSettingsPg);

    connect(chat_pg, &ChatWidget::other_profile_signal, this, &MainWindow::goToOtherProfilePg);

    connect(reg_pg, &Registration::have_an_account_signal, this, &MainWindow::goToSignIn);

    connect(other_profile_pg, &OtherProfile::goBackSignal, this, [this]() {
        goToChatPg(chat_pg->getNick());
    });

    connect(chat_pg, &ChatWidget::go_back_signal, this, &MainWindow::goToMainPg);

    connect(settings_pg, &Settings::languageChanged, this, &MainWindow::change_language);
    // connect(profile_settings_pg, &MyProfile::logOutSiganl, this, &MainWindow::login_pg);
    connect(welcome_pg, &WelcomePg::languageChanged, this, &MainWindow::change_language);

    this->setCentralWidget(staked_widget);
    this->setFixedSize(400, 700);
}

MainWindow::~MainWindow() {}

void MainWindow::goToSignIn()
{
    staked_widget->setCurrentIndex(1);
}

void MainWindow::goToRegPg()
{
    staked_widget->setCurrentIndex(2);
}

void MainWindow::goToWelcomePg() {
    staked_widget->setCurrentIndex(0);
}

void MainWindow::goToMainPg() {
    staked_widget->setCurrentIndex(3);
}

void MainWindow::goToChatPg(QString nick)
{
    // if (staked_widget->indexOf(chat_pg) != -1) {
        // staked_widget->removeWidget(chat_pg);
    // }
    // chat_pg = new ChatWidget(nick);
    // staked_widget->insertWidget(4, chat_pg);
    chat_pg->setNick(nick);
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


