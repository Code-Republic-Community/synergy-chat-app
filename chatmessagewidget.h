#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDateTime>
#include <QTextEdit>

class ChatMessageWidget : public QWidget {
    Q_OBJECT

public:
    explicit ChatMessageWidget(const QString& text, QWidget* parent = nullptr);

    void setMessage(const QString& text);

private:
    QLabel* messageLabel;
    QLabel* timeLabel;
    QVBoxLayout* layout;
};

#endif // CHATMESSAGEWIDGET_H
