#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include <QDateTime>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class ChatMessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatMessageWidget(const QString &text, QWidget *parent = nullptr);

    void setMessage(const QString &text);

private:
    QLabel *messageLabel;
    QLabel *timeLabel;
    QVBoxLayout *layout;

public:
    void setAlignment(Qt::Alignment alignment);
};

#endif // CHATMESSAGEWIDGET_H
