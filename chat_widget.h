#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include "httpclient.h"
#include "v_chat_widget.h"

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget() = default;
    void setLanguage();

public slots:
    void handleDataFromMainPage(QString nickname, QString name, QString surname, QPixmap photo);

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
    QTimer *chatReloadTimer;

private:
    void addMessage(const QString &message, bool isOutgoing);
    void clearMessages();
    void loadChat(QString nickname);
signals:
    void other_profile_signal(QString nickname, QString name, QString surname, QPixmap photo);
    void go_back_signal();
private slots:
    void handle_profile_signal(QString nickname, QString name, QString surname, QPixmap photo);
    void handle_go_back();
    void sendMessage(bool isOutgoing);
    void handle_line();
};

#endif // CHAT_WIDGET_H
