#include "chat_widget.h"

ChatWidget::ChatWidget(QString nick, QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);

    v_user = new VChatWidget("username", nick);
    v_user->resize(200, 60);

    QHBoxLayout *layout = new QHBoxLayout();
    QLineEdit *line = new QLineEdit();
    line->setPlaceholderText("Type message...");
    line->setFixedHeight(35);

    send_btn = new QPushButton();
    // send_btn->setText("🟣");
    send_btn->setFixedSize(50, 35);

    layout->addWidget(line);
    layout->addWidget(send_btn, 0, Qt::AlignLeft);
    QFrame *messagesFrame = new QFrame(this);
    messagesFrame->setFrameShape(QFrame::Box);
    messagesFrame->setStyleSheet("background-color: #420242;");
    messagesFrame->setFixedSize(380, 570);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addWidget(messagesFrame);

    QHBoxLayout *for_user = new QHBoxLayout();

    backButton = new QPushButton();
    backButton->setFixedSize(100, 40);
    backButton->setStyleSheet("background-color: #5A005A; color: white; font-weight: bold; border-radius: 5px;");

    for_user->addWidget(backButton);
    for_user->addWidget(v_user);

    connect(v_user, &VChatWidget::clicked_vchat, this, &ChatWidget::handle_profile_signal);
    connect(backButton, &QPushButton::clicked, this, &ChatWidget::handle_go_back);

    setLanguage();
    mainLayout->addLayout(for_user);
    setLayout(mainLayout);
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
    send_btn->setText("🟣");
    backButton->setText("Back");
}
