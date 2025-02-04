#include "scroll_widget.h"

ScrollWidget::ScrollWidget(QWidget *parent)
    : QWidget(parent) {
    scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    change_sizes(50, 100, 300, 600);
    // scroll->setGeometry(50, 100, 300, 600);
    scroll->setBackgroundRole(QPalette::Dark);
    scroll->setWidgetResizable(true);

    scroll_content = new QWidget(scroll);

    contentLayout = new QVBoxLayout(scroll_content);
    contentLayout->setSpacing(2);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // for (int i = 0; i < 30; ++i) {
    //     VChatWidget* chat = new VChatWidget("Avtandir3000", "@nick");
    //     add_chat(chat);
    // }

    // show_chats();

    scroll_content->setLayout(contentLayout);
    scroll->setWidget(scroll_content);
}

void ScrollWidget::add_chat(VChatWidget *new_chat)
{
    all_chats.append(new_chat);
}

void ScrollWidget::show_chats()
{
    for (int i = 0; i < all_chats.size(); ++i) {
        contentLayout->addWidget(all_chats[i]);
        all_chats[i]->scroll_long_text(all_chats[i]->get_name());

        if (i == 10) {
            draw_line("Unknown contacts");
        }
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
    QWidget *lineContainer = new QWidget();
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
}



VChatWidget* ScrollWidget::search(QString text)
{
    for (auto item : all_chats) {
        if (item->get_nick() == text) {
            return item;
        }
    }
    return nullptr;
}
