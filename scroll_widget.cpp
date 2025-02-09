#include "scroll_widget.h"

ScrollWidget::ScrollWidget(QWidget *parent)
    : QWidget(parent)
{
    scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    change_sizes(50, 100, 300, 570);
    scroll->setBackgroundRole(QPalette::Dark);
    scroll->setWidgetResizable(true);

    scroll_content = new QWidget(scroll);

    contentLayout = new QVBoxLayout(scroll_content);
    contentLayout->setSpacing(2);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    scroll_content->setLayout(contentLayout);
    scroll->setWidget(scroll_content);
}

void ScrollWidget::add_chat(VChatWidget *new_chat)
{
    all_chats.append(new_chat);
}

void ScrollWidget::show_chats()
{
    hide_search_chats();
    clear_search_chats();
    qDebug()<<"Show chats Scroll Widget";
    contentLayout->setAlignment(Qt::AlignTop);
    for (int i = 0; i < all_chats.size(); ++i) {
        contentLayout->addWidget(all_chats[i]);
        all_chats[i]->scroll_long_text(all_chats[i]->get_name());
    }
}

void ScrollWidget::hide_chats()
{
    for (int i = 0; i < all_chats.size(); ++i) {
        contentLayout->removeWidget(all_chats[i]);
        all_chats[i]->setParent(nullptr);
    }
}

void ScrollWidget::change_sizes(int x, int y, int w, int h)
{
    scroll->setGeometry(x, y, w, h);
    scroll->resize(w, h);
    scroll->setFixedSize(w, h);
}

void ScrollWidget::draw_line(QString text)
{
    lineContainer = new QWidget();
    lineContainer->setFixedHeight(20);

    QHBoxLayout *lineLayout = new QHBoxLayout(lineContainer);
    lineLayout->setSpacing(5);
    lineLayout->setContentsMargins(10, 2, 10, 2);

    QLabel *lineText = new QLabel(text);
    lineText->setStyleSheet("font-weight: bold; font-size: 14px; padding: 0px; color: white;");
    lineText->setAlignment(Qt::AlignCenter);

    QFrame *lineLeft = new QFrame();
    lineLeft->setFrameShape(QFrame::HLine);
    lineLeft->setFrameShadow(QFrame::Sunken);
    lineLeft->setStyleSheet("background-color: gray;");
    lineLeft->setFixedHeight(2);

    QFrame *lineRight = new QFrame();
    lineRight->setFrameShape(QFrame::HLine);
    lineRight->setFrameShadow(QFrame::Sunken);
    lineRight->setStyleSheet("background-color: gray;");
    lineRight->setFixedHeight(2);

    lineLayout->addWidget(lineLeft);
    lineLayout->addWidget(lineText);
    lineLayout->addWidget(lineRight);

    contentLayout->addWidget(lineContainer);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


QWidget *ScrollWidget::getContentWidget() const
{
    return scroll_content;
}

void ScrollWidget::add_matched_contact(VChatWidget *chat)
{
    matched_contacts.push_back(chat);
}

void ScrollWidget::add_matched_other_users(VChatWidget *chat)
{
    matched_other_users.push_back(chat);
}

void ScrollWidget::clear_search_chats()
{
    hide_search_chats();
    for (auto chat : matched_contacts) {
        delete chat;
        chat = nullptr;
    }
    matched_contacts.resize(0);
    for (auto chat : matched_other_users) {
        delete chat;
        chat = nullptr;
    }
    matched_other_users.resize(0);
    contentLayout->removeWidget(lineContainer);
}

void ScrollWidget::show_search_chats()
{
    qDebug()<<"Show Matched contact Scroll Widget";
    contentLayout->setAlignment(Qt::AlignTop);
    for (int i = 0; i < matched_contacts.size(); ++i)
    {
        contentLayout->addWidget(matched_contacts[i]);
        matched_contacts[i]->scroll_long_text(matched_contacts[i]->get_name());
    }
    if(!matched_other_users.isEmpty())
    {
        draw_line("Unknown Contacts");
    }
    for (int i = 0; i < matched_other_users.size(); ++i)
    {
        contentLayout->addWidget(matched_other_users[i]);
        matched_other_users[i]->scroll_long_text(matched_other_users[i]->get_name());
    }
}

void ScrollWidget::hide_search_chats()
{
    for (int i = 0; i < matched_contacts.size(); ++i) {
        contentLayout->removeWidget(matched_contacts[i]);
        matched_contacts[i]->setParent(nullptr);
    }
    for (int i = 0; i < matched_other_users.size(); ++i) {
        contentLayout->removeWidget(matched_other_users[i]);
        matched_other_users[i]->setParent(nullptr);
    }
}


void ScrollWidget::clear_chats()
{
    for (auto chat : all_chats) {
        delete chat;
        chat = nullptr;
    }
    all_chats.resize(0);
}
