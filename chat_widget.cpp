#include "chat_widget.h"

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    // send_btn->setGeometry(10, 300, 30, 30);

    QHBoxLayout *layout = new QHBoxLayout();

    QLineEdit *line = new QLineEdit();
    line->setPlaceholderText("Type message...");
    line->setFixedHeight(35);

    send_btn = new QPushButton();
    send_btn->setText("🟣");
    send_btn->setFixedSize(50, 35);

    // v_user = new VChatWidget("User", "Nick");

    layout->addWidget(line);
    layout->addWidget(send_btn);


    QFrame *messagesFrame = new QFrame(this);
    messagesFrame->setFrameShape(QFrame::Box);
    messagesFrame->setStyleSheet("background-color: #420242;");
    messagesFrame->setFixedSize(380, 570);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addWidget(messagesFrame);


    QHBoxLayout *for_user = new QHBoxLayout();
    v_user = new VChatWidget("User", "Nick");
    for_user->addWidget(v_user);

    mainLayout->addLayout(for_user);
    setLayout(mainLayout);
}
