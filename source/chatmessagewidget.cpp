#include "chatmessagewidget.h"

ChatMessageWidget::ChatMessageWidget(const QString &text, QString timestamp, bool isOutgoing, QWidget *parent)
    : QWidget(parent)
    , messageLabel(new QLabel(this))
    , timeLabel(new QLabel(this))
{
    this->hide();
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);

    messageLabel->setWordWrap(true);


    timeLabel->setAlignment(Qt::AlignRight);
    timeLabel->setObjectName("timelabel");
    if(isOutgoing)
    {
        messageLabel->setStyleSheet("border: 2px solid red;");
    }

    layout->addWidget(messageLabel);
    layout->addWidget(timeLabel);

    setMessage(text, timestamp);
    messageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
}

void ChatMessageWidget::setMessage(const QString &text, QString timestamp)
{
    messageLabel->setText(text);
    timeLabel->setText(timestamp);
    adjustSize();
}

void ChatMessageWidget::setAlignment(Qt::Alignment alignment)
{
    layout->setAlignment(Qt::Alignment(alignment));
}
