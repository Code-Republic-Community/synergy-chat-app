#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QPushButton>
#include "v_chat_widget.h"
#include "scroll_widget.h"
#include "chatmessagewidget.h"

class ChatWidget : public QWidget{
    Q_OBJECT
public:
    ChatWidget(QString nick, QWidget *parent = nullptr);
    ~ChatWidget() = default;

    void setNick(QString nick);
    QString getNick() const;
private:
    QPushButton *send_btn;
    VChatWidget *v_user;
    QPushButton *go_back_btn;
    QPushButton *backButton;
    ScrollWidget *scroll_widget;
    QLineEdit *line;
    void setLanguage();
    void addMessage(const ChatMessageWidget* message, bool who_is);
signals:
    void other_profile_signal(QString nick);
    void go_back_signal();
private slots:
    void handle_profile_signal(QString nick);
    void handle_go_back();
    void sendMessage();
    void handle_line();

private:
    void addMessageToChat(const QString& message);
};

#endif // CHAT_WIDGET_H
