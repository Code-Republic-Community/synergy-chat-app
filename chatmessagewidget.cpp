#include "chatmessagewidget.h"

ChatMessageWidget::ChatMessageWidget(const QString& text, QWidget* parent)
    : QWidget(parent), messageLabel(new QLabel(this)), timeLabel(new QLabel(this)) {

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);

    messageLabel->setWordWrap(true);
    messageLabel->setStyleSheet("background-color: #8e15de; padding: 10px; border-radius: 10px;");

    timeLabel->setAlignment(Qt::AlignRight);
    timeLabel->setStyleSheet("font-size: 10px; color: white;");

    layout->addWidget(messageLabel);
    layout->addWidget(timeLabel);

    setMessage(text);
}

void ChatMessageWidget::setMessage(const QString& text) {
    messageLabel->setText(text);
    timeLabel->setText(QDateTime::currentDateTime().toString("hh:mm"));

    adjustSize();
}
