#ifndef SCROLL_WIDGET_H
#define SCROLL_WIDGET_H

#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include "chatmessagewidget.h"
#include "v_chat_widget.h"

class MainPageWindow;

class ScrollWidget : public QWidget
{
    Q_OBJECT
public:
    ScrollWidget(QWidget *parent = nullptr);
    ~ScrollWidget() = default;

private:
    QScrollArea *scroll;
    QVBoxLayout *contentLayout;
    QWidget *scroll_content;
    QWidget *lineContainer;
private:
    void createLineContainer();
    void clearLayout(QLayout *layout);
public:
    void add_chat(VChatWidget *new_chat);
    void clear_chats();
    void show_chats();
    void change_sizes(int x, int y, int w, int h);
    QWidget *getContentWidget() const;

private:
    QVector<VChatWidget *> all_chats;
    QVector<VChatWidget*> matched_contacts;
    QVector<VChatWidget*> matched_other_users;
public:
    void add_matched_contact(VChatWidget* chat);
    void add_matched_other_users(VChatWidget* chat);
    void clear_search_chats();
    void show_search_chats();
    void delete_all_chats();
    void delete_search_chats();

public:
    void draw_line();
    void remove_Line();
};

#endif // SCROLL_WIDGET_H
