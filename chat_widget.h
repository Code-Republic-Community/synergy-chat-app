#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include "chatmessagewidget.h"
#include "httpclient.h"
#include "scroll_widget.h"
#include "v_chat_widget.h"

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    ChatWidget(QString nick, QWidget *parent = nullptr);
    ~ChatWidget() = default;

    void setNick(QString nick);
    QString getNick() const;
    void setLanguage();

private:
    QPushButton *send_btn;
    VChatWidget *v_user;
    QPushButton *go_back_btn;
    QPushButton *backButton;
    QScrollArea *scroll;
    QLineEdit *line;
    QWidget *contentWidget;
    HttpClient *chat_client;
    int x;
    int y;

private:
    void addMessage(const QString &message, bool isOutgoing);
    void clearMessages();
signals:
    void other_profile_signal(QString nick);
    void go_back_signal();
private slots:
    void handle_profile_signal(QString nick);
    void handle_go_back();
    void sendMessage(bool isOutgoing);
    void handle_line();
};

#endif // CHAT_WIDGET_H
