#include "chat_widget.h"
#include "globals.h"
#include "chatmessagewidget.h"
#include "qcoreevent.h"
#include "windows.h"
#include <QSystemTrayIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QMouseEvent>

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    chat_client = new HttpClient();
    y = 20;
    x = 200;

    chatReloadTimer = new QTimer(this);
    chatReloadTimer->setInterval(10000);

    send_btn = new QPushButton(this);
    send_btn->setGeometry(330, 20, 50, 50);
    send_btn->setFocusPolicy(Qt::NoFocus);
    send_btn->setObjectName("send_btn");
    send_btn->setIcon(QPixmap(":/pngs/send.png"));
    QSize size(40, 40);
    send_btn->setIconSize(size);
    send_btn->installEventFilter(this);

    scroll = new QScrollArea(this);
    scroll->setGeometry(20, 90, 360, 530);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    contentWidget = new QWidget();
    contentWidget->setGeometry(20, 90, 360, 530);

    scroll->setWidget(contentWidget);

    backButton = new QPushButton(this);
    backButton->setGeometry(20, 640, 100, 40);
    backButton->setFocusPolicy(Qt::NoFocus);
    // backButton->setStyleSheet(
    //     "background-color: #8e15de; color: white; font-weight: bold; border-radius: 5px;");

    line = new QLineEdit(this);
    line->setObjectName("messageInput");
    line->setGeometry(20, 20, 290, 50);

    connect(backButton, &QPushButton::clicked, this, &ChatWidget::handle_go_back);
    connect(send_btn, &QPushButton::clicked, this, &ChatWidget::sendMessage);
    connect(line, &QLineEdit::returnPressed, this, &ChatWidget::handle_line);
    setLanguage();
}


void ChatWidget::handle_profile_signal(QString nickname, QString name, QString surname, QPixmap photo)
{
    emit other_profile_signal(nickname, name, surname, photo);
}

void ChatWidget::handle_go_back()
{
    disconnect(v_user, &VChatWidget::clicked_vchat, this, &ChatWidget::handle_profile_signal);
    disconnect(chatReloadTimer, &QTimer::timeout, nullptr, nullptr);
    delete v_user;
    clearMessages();
    emit go_back_signal();
}

void ChatWidget::setLanguage()
{
    backButton->setText(tr("Back"));
    line->setPlaceholderText(tr("Type message..."));
}

void ChatWidget::handleDataFromMainPage(QString nickname, QString name, QString surname, QPixmap photo)
{
    qDebug() <<"handleDataFromMainPage(QString nickname, QString name, QString surname, QPixmap photo)";
    v_user = new VChatWidget(name, nickname, surname, photo, this);
    v_user->setGeometry(140, 630, 240, 60);
    v_user->show();
    connect(v_user, &VChatWidget::clicked_vchat, this, &ChatWidget::handle_profile_signal);
    connect(chatReloadTimer, &QTimer::timeout, this, [=]() {
        loadChat(v_user->get_nick());
    });
    chatReloadTimer->start();
    loadChat(v_user->get_nick());
}

void ChatWidget::sendMessage(bool isOutgoing)
{

    QString contact_nickname = v_user->get_nick();
    QString url_path = QString("https://synergy-iauu.onrender.com/sendMessage/%1/%2")
                           .arg(Globals::getInstance().getUserId())
                           .arg(contact_nickname);
    QUrl url(url_path);
    QJsonObject jsonData;
    jsonData["sender"] = Globals::getInstance().getUserId();
    jsonData["text"] = line->text();
    jsonData["send_date"] = QDate::currentDate().toString(Qt::ISODate);

    qDebug() << "Sending message to:" << url.toString();
    qDebug() << "Message content:" << jsonData;
    if (!line->text().isEmpty()) {
        addMessage(line->text(), isOutgoing);
        line->clear();
    }
    else
    {
        return;
    }

    connect(chat_client, &HttpClient::responseReceived, this, [=](QByteArray responseData) {
        qDebug() << "Message sent. Reloading chat.";
        loadChat(contact_nickname);
    });

    chat_client->postRequest(url, jsonData);
}

void ChatWidget::addMessage(const QString &message_text, bool isOutgoing)
{
    ChatMessageWidget *message = new ChatMessageWidget(message_text);
    message->hide();
    int messageWidth = message->width() + 20;
    int messageHeight = message->height() + 20;

    if (isOutgoing) {
        message->setGeometry(380 - messageWidth - 40, y, messageWidth, messageHeight);
    } else {
        message->setGeometry(20, y, messageWidth, messageHeight);
    }

    y += messageHeight + 10;
    int newContentHeight = y + messageHeight + 20;
    message->setParent(contentWidget);
    message->show();
    contentWidget->setMinimumHeight(newContentHeight);
}

void ChatWidget::clearMessages()
{
    QList<QWidget *> children = contentWidget->findChildren<QWidget *>();
    for (QWidget *child : children) {
        child->deleteLater();
    }
    y = 20;
    contentWidget->setMinimumHeight(0);
}

void ChatWidget::loadChat(QString nickname)
{
    QString url_path = QString("https://synergy-iauu.onrender.com/getMessageHistory/%1/%2")
                           .arg(Globals::getInstance().getUserId())
                           .arg(nickname);
    QUrl url(url_path);

    disconnect(chat_client, &HttpClient::responseReceived, nullptr, nullptr);

    connect(chat_client, &HttpClient::responseReceived, this, [=](QByteArray responseData) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isObject()) {
            qDebug() << "Invalid JSON response in loadChat";
            return;
        }

        QJsonObject jsonObject = jsonDoc.object();
        if (!jsonObject.contains("conversation") || !jsonObject["conversation"].isArray()) {
            qDebug() << "No conversation data found";
            return;
        }

        QJsonArray messagesArray = jsonObject["conversation"].toArray();
        QString currentUserId = Globals::getInstance().getUserId();
        clearMessages();
        static int messagecount = messagesArray.size();
        for (const QJsonValue &messageVal : messagesArray) {
            if (!messageVal.isObject())
                continue;

            QJsonObject messageObj = messageVal.toObject();
            QString sender = messageObj["sender"].toString();
            QString text = messageObj["text"].toString();
            bool isOutgoing = (sender == currentUserId);

            addMessage(text, isOutgoing);
            if(messageVal == messagesArray.last())
            {
                if(!isOutgoing && notification_OnOff && messagecount != messagesArray.size())
                {
                    playNotificationSound();
                    messagecount = messagesArray.size();
                }
            }
        }
    });

    chat_client->getRequest(url.toString());
}

void ChatWidget::playNotificationSound() {
    MessageBeep(MB_ICONASTERISK);
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->show();
    trayIcon->showMessage("Notification", "This is a system notification!",
                          QSystemTrayIcon::Information, 3000);
}

void ChatWidget::handle_notification_mode(bool mode)
{
    notification_OnOff = mode;
}

void ChatWidget::handle_line()
{
    QString searchText = line->text();
    if (!searchText.isEmpty()) {
        qDebug() << "Search query:" << searchText;
    }
    else {
        qDebug() << "Search query is empty";
    }
}

bool ChatWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == send_btn) {
        if (event->type() == QEvent::Enter) {
            send_btn->setIcon(QPixmap(":/pngs/send-hover.png"));
        } else if (event->type() == QEvent::Leave) {
            send_btn->setIcon(QPixmap(":/pngs/send.png"));
        }
    }
    return QWidget::eventFilter(obj, event);
}

