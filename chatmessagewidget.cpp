#include "chatmessagewidget.h"

ChatMessageWidget::ChatMessageWidget(const QString &text, QWidget *parent)
    : QWidget(parent)
    , messageLabel(new QLabel(this))
    , timeLabel(new QLabel(this))
{

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);

    messageLabel->setWordWrap(true);

    timeLabel->setAlignment(Qt::AlignRight);
    timeLabel->setObjectName("timelabel");


    layout->addWidget(messageLabel);
    layout->addWidget(timeLabel);

    setMessage(text);
    messageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
}

void ChatMessageWidget::setMessage(const QString &text)
{
    messageLabel->setText(text);
    timeLabel->setText(QDateTime::currentDateTime().toString("hh:mm"));

    adjustSize();
}

void ChatMessageWidget::setAlignment(Qt::Alignment alignment)
{
    layout->setAlignment(Qt::Alignment(alignment));
}
