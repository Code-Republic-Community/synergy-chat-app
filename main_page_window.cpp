#include "main_page_window.h"
#include "globals.h"
#include <QJsonArray>
#include <QJsonDocument>

MainPageWindow::MainPageWindow(QWidget *parent)
    : QWidget(parent)
{
    scroll_widget = new ScrollWidget(this);
    SearchButton = new QPushButton(this);
    ProfileButton = new QPushButton(this);
    searchBar = new QLineEdit(this);
    chat = new QLabel(this);

    scroll_widget->setObjectName("scroll_widget");

    overlay = new LoadingOverlay(this);
    overlay ->hideOverlay();

    QPixmap searchIcon(":/pngs/searchicon.png");
    QPixmap profileIcon(VChatWidget::cut_photo(QPixmap(), 40));
    client_main_page = new HttpClient();

    SearchButton->setGeometry(340, 10, 45, 45);
    SearchButton->setIcon(searchIcon);
    SearchButton->installEventFilter(this);
    SearchButton->setFocusPolicy(Qt::NoFocus);
    SearchButton->setIconSize(SearchButton->size());
    // SearchButton->setStyleSheet("QPushButton {"
    //                             "    border: none;"
    //                             "    background-color: transparent;"
    //                             "}"
    //                             "QPushButton:hover {"
    //                             "    background-color: transparent;"
    //                             "}"
    //                             "QPushButton:pressed {"
    //                             "    background-color: transparent;"
    //                             "}");

    searchBar->setGeometry(70, 10, 255, 50);
    // searchBar->setStyleSheet("QLineEdit {"
    //                          "    border: 3px solid #0078D7;"
    //                          "    border-radius: 10px;"
    //                          "    padding: 5px;"
    //                          "    background-color: #D4D4D6;"
    //                          "    font-size: 14px;"
    //                          "    font-family: 'Century Gothic', sans-serif;"
    //                          "    color: #333333;"
    //                          "}"
    //                          "QLineEdit:hover {"
    //                          "    border: 3px solid #005A9E;"
    //                          "}"
    //                          "QLineEdit:focus {"
    //                          "    border: 3px solid #005A9E;"
    //                          "    background-color: #f5f5f5;"
    //                          "}");

    chat->setGeometry(150, 70, 100, 30);
    // chat->setStyleSheet("color: #0078D7; font-size: 16px; font-family: 'Century Gothic', "
    //                     "sans-serif; font-weight: bold;");
    chat->setAlignment(Qt::AlignCenter);

    ProfileButton->setGeometry(10, 10, 50, 50);
    ProfileButton->installEventFilter(this);
    ProfileButton->setFocusPolicy(Qt::NoFocus);
    ProfileButton->setIcon(profileIcon);
    ProfileButton->setIconSize(profileIcon.size());
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
    connections();
    setLanguage();
}

void MainPageWindow::setLanguage()
{
    chat->setText(tr("Chats"));
    searchBar->setPlaceholderText(tr("search..."));
}

void MainPageWindow::connections()
{
    connect(ProfileButton, &QPushButton::clicked, this, &MainPageWindow::handleProfileButton);

    connect(SearchButton, &QPushButton::clicked, this, &MainPageWindow::handleSearch);

    connect(searchBar, &QLineEdit::returnPressed, this, &MainPageWindow::handleSearch);

    connect(this, &MainPageWindow::contact_successfully_added_to_scrollWidget, this, [this]() {
        scroll_widget->clear_chats();
        auto ptr = contacts.back();
        scroll_widget->add_chat(ptr);
        connect(ptr, &VChatWidget::clicked_vchat, this, &MainPageWindow::handle_vchat_click);
        scroll_widget->show_chats();
    });

    connect(this, &MainPageWindow::matched_contact_or_other_user_added_successfully, this, [this](bool contact_or_otherUser) {
                scroll_widget->clear_search_chats();
                if (contact_or_otherUser) {
                    auto ptr = matched_contacts.back();
                    scroll_widget->add_matched_contact(ptr);
                    qDebug() << "Added matched contact to scroll widget" << ptr->get_nick();
                    connect(ptr, &VChatWidget::clicked_vchat, this, &MainPageWindow::vchat_clicked_from_search_pg);
                } else {
                    auto ptr = matched_other_users.back();
                    scroll_widget->add_matched_other_users(ptr);
                    qDebug() << "Added matched other user to scroll widget" << ptr->get_nick();
                    connect(ptr, &VChatWidget::clicked_vchat, this, &MainPageWindow::vchat_clicked_from_search_pg);
                }
                scroll_widget->show_search_chats();
            });
}

void MainPageWindow::handle_vchat_click(QString nickname, QString name, QString surname, QPixmap photo)
{
    qDebug() << "Opening chat with: " << nickname;
    scroll_widget->clear_search_chats();
    scroll_widget->delete_search_chats();
    emit vchat_clicked_from_main_pg(nickname, name, surname, photo);
}

void MainPageWindow::vchat_clicked_from_search_pg(QString nickname, QString name, QString surname, QPixmap photo)
{

    if (contacts_nicknames_to_get_account_info.contains(nickname)) {
        clear_matched_arrays();
        scroll_widget->clear_chats();
        scroll_widget->delete_all_chats();
        handle_vchat_click(nickname, name, surname, photo);
        get_contacts_info_and_show();
        return;
    }
    overlay->showOverlay();
    disconnect(client_main_page, &HttpClient::responseReceived, nullptr, nullptr);
    connect(client_main_page, &HttpClient::responseReceived, this, [=](QByteArray responseData) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();
        if (jsonObject.contains("message")
            && jsonObject["message"].toString() == "Contact added successfully to both users") {
            qDebug() << "Contact added successfully: " << nickname;
            contacts_nicknames_to_get_account_info.append(nickname);
            clear_matched_arrays();
            scroll_widget->clear_chats();
            scroll_widget->delete_all_chats();
            overlay->hideOverlay();
            handle_vchat_click(nickname, name, surname, photo);
            get_contacts_info_and_show();
        } else {
            overlay->hideOverlay();
            qDebug() << "Failed to add contact: " << jsonObject.value("detail").toString();
        }

    });

    QString addContactRequestLink = "https://synergy-iauu.onrender.com/addcontact/";
    QJsonObject jsonData;
    jsonData["user_id"] = Globals::getInstance().getUserId();
    jsonData["contact_nickname"] = nickname;
    qDebug() << "Sending request to:" << addContactRequestLink;
    qDebug() << "Request body:" << nickname;
    client_main_page->postRequest(addContactRequestLink, jsonData);
}

void MainPageWindow::handleProfileButton()
{
    qDebug() << "profile button";
    emit profile_button_signal();
}

void MainPageWindow::handle_profile_photo_change(QPixmap photo)
{
    ProfileButton->setIcon(VChatWidget::cut_photo(photo, 40));
}

void MainPageWindow::handleIdReceiving()
{
    overlay->showOverlay();

    disconnect(client_main_page, &HttpClient::responseReceived, this, &MainPageWindow::handle_contact);
    connect(client_main_page, &HttpClient::responseReceived, this, &MainPageWindow::handle_contact);

    QString link("https://synergy-iauu.onrender.com/profile_info/");
    QUrl accountinfo(link + Globals::getInstance().getUserId());
    client_main_page->getRequest(accountinfo);
}

void MainPageWindow::clearDataOnLogout()
{
    clear_contact_array();
    clear_matched_arrays();

    contacts_nicknames_to_get_account_info.clear();
    scroll_widget->clear_chats();
    scroll_widget->clear_search_chats();
    scroll_widget->delete_all_chats();
    scroll_widget->delete_search_chats();
    disconnect(client_main_page, &HttpClient::responseReceived, nullptr, nullptr);

    searchBar->setText("");
}

void MainPageWindow::handleContactReDonwnload()
{
    scroll_widget->clear_search_chats();
    scroll_widget->delete_search_chats();
    scroll_widget->clear_chats();
    scroll_widget->delete_all_chats();
    contacts_nicknames_to_get_account_info.clear();
    clear_contact_array();
    clear_matched_arrays();
}

void MainPageWindow::clear_matched_arrays()
{
    matched_contacts.clear();
    matched_other_users.clear();
    // matched_contacts.resize(0);
    // matched_other_users.resize(0);
}

void MainPageWindow::clear_contact_array()
{
    contacts.clear();
    contacts.resize(0);
}

void MainPageWindow::handle_contact(QByteArray responseData)
{
    disconnect(client_main_page, &HttpClient::responseReceived, this, &MainPageWindow::handle_contact);

    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (jsonResponse.isNull() || !jsonResponse.isObject()) {
        qDebug() << "Invalid JSON response.";
        return;
    }

    QJsonObject jsonObject = jsonResponse.object();
    QString encodedPhoto = jsonObject.value("profile_photo").toString();
    profile_picture = Globals::getInstance().decodeBase64ToPixmap(encodedPhoto);
    ProfileButton->setIcon(VChatWidget::cut_photo(profile_picture, 40));

    QJsonValue contactsValue = jsonObject.value("contacts");
    if (!contactsValue.isArray()) {
        qDebug() << "Contacts field is not a JSON array!";
        return;
    }

    contacts_nicknames_to_get_account_info.clear();
    scroll_widget->clear_chats();

    QJsonArray contactsArray = contactsValue.toArray();
    for (const QJsonValue &value : contactsArray) {
        if (value.isObject()) {
            QJsonObject contactObject = value.toObject();
            QString nickname = contactObject.value("nickname").toString();  // //arden grel ei taki funkcian u liqy tex ogtagorcel dra hamar
            QString name = contactObject.value("name").toString();          // // + mi request el hajord funckiayum
            QString surname = contactObject.value("surname").toString();

            if (!nickname.isEmpty()) {
                contacts_nicknames_to_get_account_info.append(nickname);
                qDebug() << "Contact:" << nickname << "(" << name << surname << ")";
            }
        }
    }
    qDebug() << "Extracted nicknames: " << contacts_nicknames_to_get_account_info;
    disconnect(this, &MainPageWindow::received_contacts, this, &MainPageWindow::get_contacts_info_and_show);
    connect(this, &MainPageWindow::received_contacts, this, &MainPageWindow::get_contacts_info_and_show);
    emit received_contacts();
}

void MainPageWindow::get_contacts_info_and_show()
{
    disconnect(this, &MainPageWindow::received_contacts, this, &MainPageWindow::get_contacts_info_and_show);
    int count_of_contacts = contacts_nicknames_to_get_account_info.size();
    qDebug() << "count_of_contacts = contacts_nicknames_to_get_account_info.size();"
             << count_of_contacts;
    disconnect(client_main_page, &HttpClient::responseReceived, nullptr, nullptr);
    connect(client_main_page, &HttpClient::responseReceived, this, [this](QByteArray responseData) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isObject() && jsonDoc.object().value("nickname").toString().isEmpty()) {
            qDebug() << "Invalid JSON response in get_contacts_info_and_show";
            return;
        }

        QJsonObject jsonObject = jsonDoc.object();
        QString name = jsonObject.value("name").toString();
        QString surname = jsonObject.value("surname").toString();
        QString nickname = jsonObject.value("nickname").toString();
        QString encodedPhoto = jsonObject.value("profile_photo").toString();
        QPixmap photo = Globals::getInstance().decodeBase64ToPixmap(encodedPhoto);
// //////////////////////////////////
        if(!nickname.isEmpty())
        {
            qDebug() << "Received Contact Info - Name:" << name << ", Surname:" << surname << "Nickname:" << nickname;
            contacts.push_back(new VChatWidget(name, nickname, surname, photo));
            emit contact_successfully_added_to_scrollWidget();
        }
    });
    for (int i = 0; i < count_of_contacts; ++i) {
        QString contactInfoGetRequestLink = "https://synergy-iauu.onrender.com/getContactInfo/"
                                            + Globals::getInstance().getUserId() + "/"
                                            + contacts_nicknames_to_get_account_info[i];
        client_main_page->getRequest(contactInfoGetRequestLink);
    }


    overlay->hideOverlay();
}


void MainPageWindow::handle_search_data(QByteArray responseData)
{
    disconnect(client_main_page,
               &HttpClient::responseReceived,
               this,
               &MainPageWindow::handle_search_data);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (!jsonDoc.isObject()) {
        qWarning() << "Invalid JSON response.";
        return;
    }
    QJsonObject jsonObject = jsonDoc.object();
    clear_matched_arrays();

    if (jsonObject.contains("matched_contacts") && jsonObject["matched_contacts"].isArray()) {
        QJsonArray matchedContactsArray = jsonObject["matched_contacts"].toArray();
        qDebug() << "QJsonArray matchedContactsArray size = ======= " << matchedContactsArray.size() ;
        if(matchedContactsArray.size() != 0){
            for (int i = 0; i < matchedContactsArray.size(); ++i) {
                QJsonValue val = matchedContactsArray[i];
                if (val.isObject() && !val.isNull()) {
                    QJsonObject userObj = val.toObject();
                    QString nickname = userObj.value("nickname").toString();
                    QString name = userObj.value("name").toString();
                    QString surname = userObj.value("surname").toString();
                    QString encodedPhoto = userObj.value("profile_photo").toString();

                    QPixmap photo = Globals::getInstance().decodeBase64ToPixmap(encodedPhoto);
                    VChatWidget *contactWidget = new VChatWidget(name, nickname, surname, photo);
                    matched_contacts.push_back(contactWidget);
                    emit matched_contact_or_other_user_added_successfully(true);
                }
            }
        }
    }

    if (jsonObject.contains("matched_other_users") && jsonObject["matched_other_users"].isArray()) {
        QJsonArray matchedOtherUsersArray = jsonObject["matched_other_users"].toArray();
        if(matchedOtherUsersArray.size() != 0)
        {
            for (int i = 0; i < matchedOtherUsersArray.size(); ++i) {
                qDebug() << "QJsonArray matchedOtherUsersArray size = ======= " <<matchedOtherUsersArray.size() ;
                QJsonValue val = matchedOtherUsersArray[i];
                if (val.isObject()) {
                    QJsonObject userObj = val.toObject();
                    QString nickname = userObj.value("nickname").toString();
                    QString name = userObj.value("name").toString();
                    QString surname = userObj.value("surname").toString();
                    QString encodedPhoto = userObj.value("profile_photo").toString();

                    QPixmap photo = Globals::getInstance().decodeBase64ToPixmap(encodedPhoto);
                    VChatWidget *userWidget = new VChatWidget(name, nickname, surname, photo);
                    matched_other_users.push_back(userWidget);
                    emit matched_contact_or_other_user_added_successfully(false);
                }
            }
        }
    }

    qDebug() << "Matched Contacts:" << matched_contacts.size();
    qDebug() << "Matched Other Users:" << matched_other_users.size();

    if (!(matched_contacts.isEmpty() && matched_other_users.isEmpty())) {
        searchBar->setText("");
        clear_contact_array();
        scroll_widget->clear_chats();
        scroll_widget->show_search_chats();
        qDebug() << "        scroll_widget->show_search_chats(); ----------------------";
        overlay->hideOverlay();
    }
    else
    {
        scroll_widget->delete_search_chats();
        scroll_widget->clear_search_chats();
        clear_matched_arrays();
        get_contacts_info_and_show();
        searchBar->setText("");
        qDebug() << "        scroll_widget->show(); ----------------------";
        scroll_widget->show();
        overlay->hideOverlay();
    }
}


void MainPageWindow::handleSearch()
{
    QString searchText = searchBar->text();
    if (!searchText.isEmpty()) {
        overlay->showOverlay();
        scroll_widget->clear_chats();
        scroll_widget->delete_search_chats();
        scroll_widget->delete_all_chats();
        clear_matched_arrays();
        qDebug() << "Search query:" << searchText;
        QString searchLink = "https://synergy-iauu.onrender.com/search/"
                             + Globals::getInstance().getUserId() + "/" + searchText;
        disconnect(client_main_page,
                &HttpClient::responseReceived,
                this,
                &MainPageWindow::handle_search_data);
        connect(client_main_page,
                &HttpClient::responseReceived,
                this,
                &MainPageWindow::handle_search_data,
                Qt::UniqueConnection);
        client_main_page->getRequest(searchLink);
    } else {
        qDebug() << "Search query is empty";
    }
}


bool MainPageWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == SearchButton) {
        if (event->type() == QEvent::Enter) {
            SearchButton->setIcon(QPixmap(":/pngs/searchicon-hover.png"));
            SearchButton->setIconSize(SearchButton->size());
        } else if (event->type() == QEvent::Leave) {
            SearchButton->setIcon(QPixmap(":/pngs/searchicon.png"));
            SearchButton->setIconSize(SearchButton->size());
        }
    }
    else if (obj == ProfileButton) {
        if (event->type() == QEvent::Enter) {
            ProfileButton->setIcon(VChatWidget::cut_photo(profile_picture, 40, QColor("#005A9E")));
        } else if (event->type() == QEvent::Leave) {
            ProfileButton->setIcon(VChatWidget::cut_photo(profile_picture, 40));
        }
    }
    return QWidget::eventFilter(obj, event);
}
