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
signals:
    void other_profile_signal(QString nick);
    void go_back_signal();
private slots:
    void handle_profile_signal(QString nick);
    void handle_go_back();
};

#endif // CHAT_WIDGET_H
