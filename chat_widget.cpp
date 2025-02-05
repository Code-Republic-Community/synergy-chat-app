    // messagesFrame->setFixedSize(380, 570);

#include "chat_widget.h"
extern QByteArray globalId;


ChatWidget::ChatWidget(QString nick, QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    chat_client = new HttpClient();
    y = 20;
    x = 200;

    v_user = new VChatWidget("username", nick, this);
    v_user->setGeometry(120, 630, 272, 60);

    send_btn = new QPushButton(this);
    send_btn->setGeometry(340, 10, 50, 55);


    scroll = new QScrollArea(this);
    scroll->setGeometry(10, 70, 380, 560);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    contentWidget = new QWidget();
    contentWidget->setGeometry(0, 0, 380, 560);

    scroll->setWidget(contentWidget);

    backButton = new QPushButton(this);
    backButton->setGeometry(12, 632, 100, 55);
    backButton->setStyleSheet("background-color: #8e15de; color: white; font-weight: bold; border-radius: 5px;");

    line = new QLineEdit(this);
    line->setObjectName("messageInput");
    line->setGeometry(10, 10, 325, 55);

    connect(v_user, &VChatWidget::clicked_vchat, this, &ChatWidget::handle_profile_signal);
    connect(backButton, &QPushButton::clicked, this, &ChatWidget::handle_go_back);
    connect(send_btn, &QPushButton::clicked, this, &ChatWidget::sendMessage);
    connect(line, &QLineEdit::returnPressed, this, &ChatWidget::handle_line);

    setLanguage();
}



void ChatWidget::setNick(QString nick)
{
    clearMessages();
    v_user->set_nick(nick);
}

QString ChatWidget::getNick() const
{
    return v_user->get_nick();
}

void ChatWidget::handle_profile_signal(QString nick)
{
    emit other_profile_signal(nick);
}

void ChatWidget::handle_go_back()
{
    emit go_back_signal();
}

void ChatWidget::setLanguage()
{
    send_btn->setText(tr("✉️"));
    backButton->setText(tr("Back"));
    line->setPlaceholderText(tr("Type message..."));
}

void ChatWidget::sendMessage(bool isOutgoing)
{
    QString url_path = "http://127.0.0.1:8000/sendMessage/";
    url_path += globalId;
    QUrl url(url_path);
    QJsonObject jsonData;
    jsonData["sender"] = "115310f3-4d6e-4897-a15a-dd62bea31204";
    jsonData["text"] = line->text();

    qDebug() << "Hesa";
    chat_client->postRequest(url, jsonData);

    if (line->text() != "") {
        addMessage(line->text(), isOutgoing);
        line->clear();
    }
}

void ChatWidget::addMessage(const QString& message_text, bool isOutgoing)
{
    ChatMessageWidget *message = new ChatMessageWidget(message_text, contentWidget);
    if (isOutgoing) {
        message->setGeometry(20, y, message->width() + 20, message->height() + 20);
    } else {
        message->setGeometry(380 - message->width() - 40, y, message->width() + 20, message->height() + 20);
    }
    y += message->height();
    int newContentHeight = y + message->height() + 20;
    message->show();
    contentWidget->setMinimumHeight(newContentHeight);
}

void ChatWidget::clearMessages()
{
    QList<QWidget*> children = contentWidget->findChildren<QWidget*>();
    for (QWidget *child : children) {
        child->deleteLater();
    }
    y = 20;
    contentWidget->setMinimumHeight(0);
}


void ChatWidget::handle_line()
{
    QString searchText = line->text();
    if (!searchText.isEmpty()) {
        qDebug() << "Search query:" << searchText;
        //chat filtration, request
        // if (scroll_widget->s)
        } else {
        qDebug() << "Search query is empty";
    }
}
