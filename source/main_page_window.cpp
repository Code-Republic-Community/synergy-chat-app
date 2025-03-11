#include <QJsonArray>
#include <QJsonDocument>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "main_page_window.h"
#include "globals.h"


MainPageWindow::MainPageWindow(QWidget *parent)
    : QWidget(parent)
{
    wsClient = nullptr;
    scroll_widget = new ScrollWidget(this);
    SearchButton = new QPushButton(this);
    ProfileButton = new QPushButton(this);
    searchBar = new QLineEdit(this);
    chat = new QLabel(this);

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;

    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);
    player->setSource(QUrl("qrc:/mp3/notification.wav"));


    notification_OnOff = true;

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

    searchBar->setGeometry(70, 10, 255, 50);

    chat->setGeometry(150, 70, 100, 30);
    chat->setAlignment(Qt::AlignCenter);

    ProfileButton->setGeometry(10, 10, 50, 50);
    ProfileButton->installEventFilter(this);
    ProfileButton->setFocusPolicy(Qt::NoFocus);
    ProfileButton->setIcon(profileIcon);
    ProfileButton->setIconSize(profileIcon.size());
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
    emit vchat_clicked_from_main_pg(nickname, name, surname, photo, wsClient);
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

    QString addContactRequestLink = Globals::getInstance().getSERVER_LINK() + "addcontact/";
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

void MainPageWindow::handle_notification_mode(bool mode)
{
    notification_OnOff = mode;
}

void MainPageWindow::handleIdReceiving()
{
    overlay->showOverlay();

    disconnect(client_main_page, &HttpClient::responseReceived, this, &MainPageWindow::handle_contact);
    connect(client_main_page, &HttpClient::responseReceived, this, &MainPageWindow::handle_contact);

    QString link(Globals::getInstance().getSERVER_LINK() + "profile_info/");
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
    qDebug() << "wsClient" <<wsClient;
    if(wsClient)
    {
        disconnect(wsClient, &WebSocketClient::messageReceived, this, &MainPageWindow::handleMessageReceive);
        delete wsClient;
        wsClient = nullptr;
        qDebug() << "wsClient" << wsClient;
    }
    my_nickname = "";
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
}

void MainPageWindow::clear_contact_array()
{
    contacts.clear();
    contacts.resize(0);
}

void MainPageWindow::handleMessageReceive(const QString &sender)
{
    for(int i = 0; i < contacts.size(); ++i)
    {
        if(contacts[i]->get_nick() == sender)
        {
            emit loadChatonResponse(sender, contacts[i]);
            break;
        }
    }
    if (notification_OnOff && player->playbackState() != QMediaPlayer::PlayingState) {
        qDebug() << "Playing .wav";
        player->play();
    }

    qDebug() << "    emit loadChatonResponse(); " << "sender" << sender;
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


    my_nickname = jsonObject.value("nickname").toString();
    if(!wsClient)
    {
        wsClient = new WebSocketClient(my_nickname, this);
        qDebug() << "wsClient" << wsClient;
        disconnect(wsClient, &WebSocketClient::messageReceived, this, &MainPageWindow::handleMessageReceive);
        connect(wsClient, &WebSocketClient::messageReceived, this, &MainPageWindow::handleMessageReceive);
    }

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
            QString nickname = contactObject.value("nickname").toString();
            QString name = contactObject.value("name").toString();
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

        if(!nickname.isEmpty())
        {
            qDebug() << "Received Contact Info - Name:" << name << ", Surname:" << surname << "Nickname:" << nickname;
            contacts.push_back(new VChatWidget(name, nickname, surname, photo));
            emit contact_successfully_added_to_scrollWidget();
        }
    });
    for (int i = 0; i < count_of_contacts; ++i) {
        QString contactInfoGetRequestLink = Globals::getInstance().getSERVER_LINK()
                                            + "getContactInfo/"
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
        QString searchLink = Globals::getInstance().getSERVER_LINK() + "search/"
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
