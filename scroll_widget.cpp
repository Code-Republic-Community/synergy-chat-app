#include "scroll_widget.h"

ScrollWidget::ScrollWidget(QWidget *parent)
    : QWidget(parent)
{
    createLineContainer();
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

void ScrollWidget::createLineContainer()
{
    lineContainer = new QWidget;
    lineContainer->setFixedHeight(20);

    QHBoxLayout *lineLayout = new QHBoxLayout(lineContainer);
    lineLayout->setSpacing(5);
    lineLayout->setContentsMargins(10, 2, 10, 2);

    QLabel *lineText = new QLabel("Unknown Contacts");
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
}

void ScrollWidget::change_sizes(int x, int y, int w, int h)
{
    scroll->setGeometry(x, y, w, h);
    scroll->resize(w, h);
    scroll->setFixedSize(w, h);
}

void ScrollWidget::draw_line()
{
    contentLayout->addWidget(lineContainer);
    lineContainer->show();
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ScrollWidget::remove_Line()
{
    contentLayout->removeWidget(lineContainer);
    lineContainer->hide();
}

QWidget *ScrollWidget::getContentWidget() const
{
    return scroll_content;
}

void ScrollWidget::add_chat(VChatWidget *new_chat)
{
    all_chats.append(new_chat);
}

void ScrollWidget::add_matched_contact(VChatWidget *chat)
{
    matched_contacts.push_back(chat);
}

void ScrollWidget::add_matched_other_users(VChatWidget *chat)
{
    matched_other_users.push_back(chat);
}

void ScrollWidget::show_chats()
{
    clear_chats();
    qDebug() << "Show chats Scroll Widget" << all_chats.size();
    contentLayout->setAlignment(Qt::AlignTop);
    for (int i = 0; i < all_chats.size(); ++i) {
        contentLayout->addWidget(all_chats[i]);
        all_chats[i]->show();
        all_chats[i]->scroll_long_text(all_chats[i]->get_name());
    }
}

void ScrollWidget::clear_chats()
{
    qDebug() << "Removing all chats from Scroll Widget";
    for (auto *chat : all_chats) {
        contentLayout->removeWidget(chat);
        chat->hide();
    }
}

void ScrollWidget::clear_search_chats()
{
    for (auto *chat : matched_contacts) {
        contentLayout->removeWidget(chat);
        chat->hide();
    }
    remove_Line();
    for (auto *chat : matched_other_users) {
        contentLayout->removeWidget(chat);
        chat->hide();
    }
}

void ScrollWidget::show_search_chats()
{
    contentLayout->setAlignment(Qt::AlignTop);
    if (!matched_contacts.isEmpty()) {
        qDebug() << "Show Matched contact Scroll Widget";
        for (int i = 0; i < matched_contacts.size(); ++i) {
            contentLayout->addWidget(matched_contacts[i]);
            matched_contacts[i]->show();
            matched_contacts[i]->scroll_long_text(matched_contacts[i]->get_name());
        }
    }
    if (!matched_other_users.isEmpty()) {
        qDebug() << "Show Matched Other Users Scroll Widget";
        draw_line();
        for (int i = 0; i < matched_other_users.size(); ++i) {
            contentLayout->addWidget(matched_other_users[i]);
            matched_other_users[i]->show();
            matched_other_users[i]->scroll_long_text(matched_other_users[i]->get_name());
        }
    }
}

void ScrollWidget::delete_all_chats()
{
    qDebug() << "Deleting all chat widgets";

    for (auto *chat : all_chats) {
        contentLayout->removeWidget(chat);
        delete chat;
    }

    all_chats.clear();
}

void ScrollWidget::delete_search_chats()
{
    qDebug() << "Deleting search chat widgets";

    for (auto *chat : matched_contacts) {
        contentLayout->removeWidget(chat);
        delete chat;
    }
    matched_contacts.clear();

    for (auto *chat : matched_other_users) {
        contentLayout->removeWidget(chat);
        delete chat;
    }
    matched_other_users.clear();
}
