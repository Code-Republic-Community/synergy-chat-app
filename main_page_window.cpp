#include "main_page_window.h"

MainPageWindow::MainPageWindow(QWidget* parent):
    QWidget(parent), SearchButton(new QPushButton(this)),
    searchBar(new QLineEdit(this)), chat(new QLabel(this)){

    QPixmap searchIcon(":/pngs/profileicon");
    QPixmap profileIcon(":/pngs/profileicon");

    SearchButton->setGeometry(340, 10, 50, 50);
    SearchButton->setIcon(searchIcon);
    SearchButton->setIconSize(searchIcon.size());
    SearchButton->setStyleSheet("QPushButton {"
                                "    border: none;"
                                "    background-color: transparent;"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: rgba(142, 21, 222, 0.1);"
                                "}"
                                "QPushButton:pressed {"
                                "    background-color: rgba(142, 21, 222, 0.2);"
                                "}");
    ProfileButton = nullptr;
    // ProfileButton->setGeometry(10,10, 50, 50);
    // ProfileButton->setIcon(profileIcon);
    // ProfileButton->setIconSize(profileIcon.size());
    // ProfileButton->setStyleSheet("QPushButton {"
    //                              "    border: none;"
    //                              "    background-color: transparent;"
    //                              "}"
    //                              "QPushButton:hover {"
    //                              "    background-color: rgba(142, 21, 222, 0.1);"
    //                              "}"
    //                              "QPushButton:pressed {"
    //                              "    background-color: rgba(142, 21, 222, 0.2);"
    //                              "}");
    // ProfileButton->setEnabled(true);
    // connect(ProfileButton, &QPushButton::clicked, this, &MainPageWindow::profile_button_signal);

    searchBar->setGeometry(70, 10, 265, 50);
    searchBar->setPlaceholderText("search...");
    searchBar->setStyleSheet("QLineEdit {"
                             "    border: 2px solid #8e15de;"
                             "    border-radius: 10px;"
                             "    padding: 5px;"
                             "    background-color: #D4D4D6;"
                             "    font-size: 14px;"
                             "    font-family: 'Century Gothic', sans-serif;"
                             "    color: #333333;"
                             "}"
                             "QLineEdit:hover {"
                             "    border: 2px solid #9b30ff;"
                             "}"
                             "QLineEdit:focus {"
                             "    border: 2px solid #ba55d3;"
                             "    background-color: #f5f5f5;"
                             "}");

    chat->setText("CHATS");
    chat->setGeometry(170, 30, 100, 100);
    chat->setStyleSheet("color: #8e15de; font-size: 16px; font-family: 'Century Gothic', sans-serif; font-weight: bold;");

    scroll_widget = new ScrollWidget(this);

    for (int i = 0; i < 30; ++i) {
        VChatWidget* chat = new VChatWidget("Avtandir3000", "@nick");
        connect(chat, &VChatWidget::clicked_vchat, this, &MainPageWindow::handle_vchat_click);
        scroll_widget->add_chat(chat);
    }
    scroll_widget->show_chats();

    QPushButton *just_btn = new QPushButton(this);
    just_btn->setGeometry(170, 2, 20, 20);
    just_btn->setGeometry(10,10, 50, 50);
    just_btn->setIcon(profileIcon);
    just_btn->setIconSize(profileIcon.size());
    just_btn->setStyleSheet("QPushButton {"
                                 "    border: none;"
                                 "    background-color: transparent;"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: rgba(142, 21, 222, 0.1);"
                                 "}"
                                 "QPushButton:pressed {"
                                 "    background-color: rgba(142, 21, 222, 0.2);"
                                 "}");
    connect(just_btn, &QPushButton::clicked, this, &MainPageWindow::handleProfileButton);
}

void MainPageWindow::handle_vchat_click()
{
    emit vchat_clicked_from_main_pg();
}

void MainPageWindow::handleProfileButton()
{
    qDebug() << "profile button";
    emit profile_button_signal();
}

