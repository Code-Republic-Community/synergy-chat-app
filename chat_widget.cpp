    // messagesFrame->setFixedSize(380, 570);

#include "chat_widget.h"

ChatWidget::ChatWidget(QString nick, QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);

    v_user = new VChatWidget("username", nick, this);
    v_user->setGeometry(120, 630, 272, 60);

    send_btn = new QPushButton(this);
    send_btn->setGeometry(340, 10, 50, 55);


    scroll_widget = new ScrollWidget(this);
    scroll_widget->change_sizes(10, 70, 380, 560);

    if (true) {
        ChatMessageWidget *message = new ChatMessageWidget("Kargin Serial", this);
        message->setGeometry(70, 70, 100, 100);
    }

    backButton = new QPushButton(this);
    backButton->setGeometry(12, 632, 100, 55);
    backButton->setStyleSheet("background-color: #5A005A; color: white; font-weight: bold; border-radius: 5px;");

    line = new QLineEdit(this);
    line->setObjectName("messageInput");
    line->setPlaceholderText("Type message...");
    line->setGeometry(10, 10, 325, 55);

    connect(v_user, &VChatWidget::clicked_vchat, this, &ChatWidget::handle_profile_signal);
    connect(backButton, &QPushButton::clicked, this, &ChatWidget::handle_go_back);
    connect(send_btn, &QPushButton::clicked, this, &ChatWidget::sendMessage);
    connect(line, &QLineEdit::returnPressed, this, &ChatWidget::handle_line);

    setLanguage();
}



void ChatWidget::setNick(QString nick)
{
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
    send_btn->setText("✉️");
    backButton->setText("Back");
}

void ChatWidget::addMessage(const ChatMessageWidget *message, bool who_is)
{
    // QHBoxLayout *layout = new QHBoxLayout(this);
    // if (who_is)
}


void ChatWidget::sendMessage()
{
    QLineEdit* line = findChild<QLineEdit*>();
    if(line && !line->text().isEmpty()) {
        addMessageToChat(line->text());
        line->clear();
    }
}

void ChatWidget::addMessageToChat(const QString& message)
{
    // // Implementation depends on your ScrollWidget's API
    // // Example pseudo-code:
    // QLabel* newMsg = new QLabel(message, scroll_widget);
    // scroll_widget->addWidget(newMsg);

    // // Scroll to bottom
    // QScrollArea* scrollArea = scroll_widget->findChild<QScrollArea*>();
    // if(scrollArea) {
    //     scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    // }
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
