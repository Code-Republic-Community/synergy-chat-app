#include "mainwindow.h"

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

    staked_widget = new QStackedWidget();
    staked_widget->addWidget(welcome_pg); // 0
    staked_widget->addWidget(login_pg); // 1
    staked_widget->addWidget(reg_pg); // 2
    staked_widget->addWidget(main_pg); // 3
    staked_widget->addWidget(chat_pg); // 4
    staked_widget->addWidget(profile_settings_pg); // 5
    staked_widget->addWidget(settings_pg); // 6


    connect(welcome_pg, &WelcomePg::signInClicked, this, &MainWindow::goToSignIn);

    connect(welcome_pg, &WelcomePg::signUpClicked, this, &MainWindow::goToRegPg);
    connect(login_pg, &Login::prev_btn_signal, this, &MainWindow::goToWelcomePg);
    connect(login_pg, &Login::next_btn_signal, this, &MainWindow::goToMainPg);

    connect(reg_pg, &Registration::prev_btn_signal, this, &MainWindow::goToWelcomePg);

    connect(main_pg, &MainPageWindow::vchat_clicked_from_main_pg, this, &MainWindow::goToChatPg);

    connect(main_pg, &MainPageWindow::profile_button_signal, this, &MainWindow::goToProfileSettingsPg);

    connect(profile_settings_pg, &MyProfile::gotoSettingsSignal, this, &MainWindow::goToSettings);
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
