#ifndef MAIN_PAGE_WINDOW_H
#define MAIN_PAGE_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QVector>
#include "scroll_widget.h"
#include "v_chat_widget.h"

class MainPageWindow : public QWidget
{
    Q_OBJECT

public:
    MainPageWindow(QWidget* parent = nullptr);
    ~MainPageWindow() = default;

private:
    QPushButton* SearchButton;
    QPushButton* ProfileButton;
    QLineEdit *searchBar;
    QLabel* chat;
    ScrollWidget* scroll_widget;
    QVector<VChatWidget*> all_vchats; // tmp
public slots:
    void handle_vchat_click();
              // void handleSearchButton();
    void handleProfileButton(); //Tiko Part
signals:
    void vchat_clicked_from_main_pg();
    void profile_button_signal();
};

#endif // MAIN_PAGE_WINDOW_H
