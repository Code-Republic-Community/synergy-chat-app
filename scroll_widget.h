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

public:
    void add_chat(VChatWidget *new_chat);
    void clear_chats();
    void show_chats();
    void hide_chats();
    void change_sizes(int x, int y, int w, int h);
    QWidget *getContentWidget() const;

private:
    QVector<VChatWidget *> all_chats;

public:
    void draw_line(QString text);
private:
    VChatWidget *search(QString text);
};

#endif // SCROLL_WIDGET_H
