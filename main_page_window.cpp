#include "main_page_window.h"

MainPageWindow::MainPageWindow(QWidget* parent):
    QWidget(parent), scroll_widget(new ScrollWidget(this)), SearchButton(new QPushButton(this)),
    ProfileButton(new QPushButton(this)),
    searchBar(new QLineEdit(this)), chat(new QLabel(this)){

    QPixmap searchIcon(":/pngs/searchicon.png");
    QPixmap profileIcon(VChatWidget::cut_photo(":/pngs/panda.jpg"));

    for (int i = 0; i < 30; ++i) {
        VChatWidget* chat = new VChatWidget("Avtandir3000", "@nick" + QString::number(i));
        connect(chat, &VChatWidget::clicked_vchat, this, &MainPageWindow::handle_vchat_click);
        scroll_widget->add_chat(chat);
    }
    scroll_widget->show_chats();

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


    ProfileButton->setGeometry(10,10, 50, 50);
    ProfileButton->setIcon(profileIcon);
    ProfileButton->setIconSize(profileIcon.size());
    ProfileButton->setStyleSheet("QPushButton {"
                                 "    border: none;"
                                 "    background-color: transparent;"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: rgba(142, 21, 222, 0.1);"
                                 "}"
                                 "QPushButton:pressed {"
                                 "    background-color: rgba(142, 21, 222, 0.2);"
                                 "}");
    connect(ProfileButton, &QPushButton::clicked, this, &MainPageWindow::handleProfileButton);
    connect(SearchButton, &QPushButton::clicked, this, &MainPageWindow::handleSearchButton);
    connect(searchBar, &QLineEdit::returnPressed, this, &MainPageWindow::handleSearch);
}

void MainPageWindow::handle_vchat_click(QString nickname)
{
    emit vchat_clicked_from_main_pg(nickname);
}

void MainPageWindow::handleProfileButton()
{
    qDebug() << "profile button";
    emit profile_button_signal();
}

void MainPageWindow::handleSearchButton(){
    qDebug() << "search button";
    handleSearch();
}

void MainPageWindow::handleSearch(){
    QString searchText = searchBar->text();
    if (!searchText.isEmpty()) {
        qDebug() << "Search query:" << searchText;
        //chat filtration, request
        // if (scroll_widget->s)
    } else {
        qDebug() << "Search query is empty";
    }
}


void MainPageWindow::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        searchBar->setFocus();
    }

}
