#include "main_page_window.h"
#include "globals.h"
#include <QJsonDocument>
#include <QJsonArray>

MainPageWindow::MainPageWindow(QWidget *parent)
    : QWidget(parent)
    , scroll_widget(new ScrollWidget(this))
    , SearchButton(new QPushButton(this))
    , ProfileButton(new QPushButton(this))
    , searchBar(new QLineEdit(this))
    , chat(new QLabel(this))
{
    QPixmap searchIcon(":/pngs/searchicon.png");
    QPixmap profileIcon(VChatWidget::cut_photo(":/pngs/panda.jpg", 40));
    client_main_page = new HttpClient();

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

    chat->setGeometry(170, 30, 100, 100);
    chat->setStyleSheet("color: #8e15de; font-size: 16px; font-family: 'Century Gothic', "
                        "sans-serif; font-weight: bold;");

    ProfileButton->setGeometry(10, 10, 50, 50);
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
    connect(this, &MainPageWindow::received_contacts, this, &MainPageWindow::fill_contacts);
    connect(ProfileButton, &QPushButton::clicked, this, &MainPageWindow::handleProfileButton);
    connect(SearchButton, &QPushButton::clicked, this, &MainPageWindow::handleSearchButton);
    connect(searchBar, &QLineEdit::returnPressed, this, &MainPageWindow::handleSearch);
    setLanguage();
}

void MainPageWindow::setLanguage()
{
    chat->setText(tr("Chats"));
    searchBar->setPlaceholderText(tr("search..."));
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

void MainPageWindow::handleIdReceiving()
{
    connect(client_main_page, &HttpClient::responseReceived, this, &MainPageWindow::handle_contact);
    QString link("https://synergy-iauu.onrender.com/profile_info/");
    QUrl accountinfo(link + Globals::getInstance().getUserId());
    client_main_page->getRequest(accountinfo);
}

void MainPageWindow::handle_contact(QByteArray responseData)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (jsonResponse.isNull() || !jsonResponse.isObject()) {
        qDebug() << "Invalid JSON response.";
        return;
    }

    QJsonObject jsonObject = jsonResponse.object();
    QJsonValue contactsValue = jsonObject.value("contacts");

    if (!contactsValue.isString()) {
        qDebug() << "Contacts field is not a string!";
        return;
    }

    // Convert the stringified JSON array into a QJsonArray
    QJsonDocument contactsDoc = QJsonDocument::fromJson(contactsValue.toString().toUtf8());
    if (!contactsDoc.isArray()) {
        qDebug() << "Failed to parse contacts as a JSON array!";
        return;
    }

    QJsonArray contactsArray = contactsDoc.array();
    QStringList nicknames;

    for (const QJsonValue &value : contactsArray) {
        if (value.isString()) {
            nicknames.append(value.toString());
        }
    }
    contacts = nicknames;
    qDebug() << "Extracted nicknames: " << contacts;
    emit received_contacts();
}

void MainPageWindow::fill_contacts()
{
    scroll_widget->clear_chats();
    for (int i = 0; i < contacts.size(); ++i) {
        VChatWidget *chat = new VChatWidget("Avtandir3000", "@" + contacts[i]);
        connect(chat, &VChatWidget::clicked_vchat, this, &MainPageWindow::handle_vchat_click);
        scroll_widget->add_chat(chat);
    }
    qDebug() << contacts;
    scroll_widget->show_chats();
}




void MainPageWindow::handleSearchButton()
{
    qDebug() << "search button";
    handleSearch();
}


void MainPageWindow::handleSearch()
{
    QString searchText = searchBar->text();
    if (!searchText.isEmpty()) {
        qDebug() << "Search query:" << searchText;
        //chat filtration, request
        // if (scroll_widget->s)
        } else {
        qDebug() << "Search query is empty";
    }
}

void MainPageWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        searchBar->setFocus();
    }
}
