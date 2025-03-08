#include <QSystemTrayIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QMouseEvent>
#include <QDateTime>
#include <QTimeZone>
#include <QScrollBar>

#include "QTimeZone"
#include "chat_widget.h"
#include "globals.h"
#include "chatmessagewidget.h"
#include "qcoreevent.h"

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    chat_client = new HttpClient();
    y = 20;
    x = 200;

    send_btn = new QPushButton(this);
    send_btn->setGeometry(330, 20, 50, 50);
    send_btn->setFocusPolicy(Qt::NoFocus);
    send_btn->setObjectName("send_btn");
    send_btn->setIcon(QPixmap(":/pngs/resources/send.png"));
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
    delete v_user;
    clearMessages();
    contentWidget->setMinimumHeight(0);
    scroll->widget()->adjustSize();

    emit go_back_signal();
}

void ChatWidget::setLanguage()
{
    backButton->setText(tr("Back"));
    line->setPlaceholderText(tr("Type message..."));
}

void ChatWidget::handleDataFromMainPage(QString nickname, QString name, QString surname, QPixmap photo, WebSocketClient *ws)
{
    qDebug() <<"handleDataFromMainPage(QString nickname, QString name, QString surname, QPixmap photo)";
    wsClient = ws;

    v_user = new VChatWidget(name, nickname, surname, photo, this);
    v_user->setGeometry(140, 630, 240, 60);
    v_user->show();
    connect(v_user, &VChatWidget::clicked_vchat, this, &ChatWidget::handle_profile_signal);
    contentWidget->setMinimumHeight(0);
    scroll->widget()->adjustSize();
    loadChat(v_user->get_nick());
    connect(this, &ChatWidget::firstLoadEnded, this, [this](){
        scroll->widget()->adjustSize();
        scroll->verticalScrollBar()->setValue(scroll->verticalScrollBar()->maximum());
        disconnect(this, &ChatWidget::firstLoadEnded, nullptr, nullptr);
    });

}

void ChatWidget::handleLoadChatOnResponse(QString sender, VChatWidget* vchatptr)
{
    qDebug()<<"Chat Widget handle load chat 1";
    if(sender == v_user->get_nick())
    {
        if(this ->isVisible())
        {
            loadChat(sender);

        }
        else
        {
            vchatptr->set_unread_mode();
        }
    }
    else
    {
        vchatptr->set_unread_mode();
    }
    qDebug()<<"Chat Widget handle load chat 2";
}


void ChatWidget::sendMessage(bool isOutgoing)
{
    if (line->text().isEmpty()) {
        return;
    }
    QString text = line->text();
    QString contact_nickname = v_user->get_nick();
    QDateTime currentTime = QDateTime::currentDateTime();
    QString formattedTime = currentTime.toString("HH:mm");
    addMessage(text, isOutgoing, formattedTime);
    line->clear();
    qDebug() << "Sending message 11111111111";
    wsClient->sendMessage(contact_nickname, text);
    qDebug() << "Sending message 22222222222";
    loadChat(contact_nickname);
}


void ChatWidget::addMessage(const QString &message_text, bool isOutgoing, QString timestamp)
{
    ChatMessageWidget *message = new ChatMessageWidget(message_text, timestamp, isOutgoing);
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
    scroll->widget()->adjustSize();

}

void ChatWidget::loadChat(QString nickname)
{
    contentWidget->setMinimumHeight(0);
    scroll->widget()->adjustSize();
    QString url_path = Globals::getInstance().getSERVER_LINK() + QString("getMessageHistory/%1/%2").arg(Globals::getInstance().getUserId(), nickname);

    QUrl url(url_path);

    disconnect(chat_client, &HttpClient::responseReceived, nullptr, nullptr);

    connect(chat_client, &HttpClient::responseReceived, this, [=](QByteArray responseData) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isArray()) {
            qDebug() << "Invalid JSON response in loadChat: Expected an array";
            return;
        }

        QJsonArray messagesArray = jsonDoc.array();
        clearMessages();

        for (const QJsonValue &messageVal : messagesArray)
        {
            if (!messageVal.isObject())
            {
                continue;
            }
            QJsonObject messageObj = messageVal.toObject();
            // QString sender = messageObj["sender"].toString();
            QString recipient = messageObj["recipient"].toString();
            QString text = messageObj["message"].toString();
            QString timestamp = messageObj["timestamp"].toString();

            qint64 unixTimeSec = static_cast<qint64>(timestamp.toDouble());
            QDateTime dateTime = QDateTime::fromSecsSinceEpoch(unixTimeSec, QTimeZone::utc());
            QString formattedTime = dateTime.toString("HH:mm");

            bool isOutgoing = (recipient == v_user->get_nick());
            addMessage(text, isOutgoing, formattedTime);
        }
        scroll->widget()->adjustSize();
        emit firstLoadEnded();
    });

    chat_client->getRequest(url.toString());


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
            send_btn->setIcon(QPixmap(":/pngs/resources/send-hover.png"));
        } else if (event->type() == QEvent::Leave) {
            send_btn->setIcon(QPixmap(":/pngs/resources/send.png"));
        }
    }
    return QWidget::eventFilter(obj, event);
}

