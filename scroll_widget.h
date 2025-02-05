#ifndef SCROLL_WIDGET_H
#define SCROLL_WIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include "v_chat_widget.h"
#include <QVector>
#include "chatmessagewidget.h"
#include <QScrollBar>

class MainPageWindow;

class ScrollWidget : public QWidget {
    Q_OBJECT
public:
    ScrollWidget(QWidget *parent = nullptr);
    ~ScrollWidget() = default;
private:
    QScrollArea *scroll;
    QVBoxLayout *contentLayout;
    QWidget* scroll_content;
public:
    void add_chat(VChatWidget *new_chat);
    void show_chats();
    void change_sizes(int x, int y, int w, int h);
    QWidget* getContentWidget() const;
private:
    QVector<VChatWidget*> all_chats;
private:
    void draw_line(QString text);
    VChatWidget* search(QString text);
};

#endif // SCROLL_WIDGET_H
