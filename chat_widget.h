#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "v_chat_widget.h"

class ChatWidget : public QWidget{
    Q_OBJECT
public:
    ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget() = default;
private:
    QPushButton *send_btn;
    VChatWidget *v_user;
};

#endif // CHAT_WIDGET_H
