#ifndef MAIN_PAGE_WINDOW_H
#define MAIN_PAGE_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QVector>
#include <QKeyEvent>
#include "scroll_widget.h"
#include "v_chat_widget.h"

class MainPageWindow : public QWidget
{
    Q_OBJECT

public:
    MainPageWindow(QWidget* parent = nullptr);
    ~MainPageWindow() = default;
    void setLanguage();
private:
    ScrollWidget* scroll_widget;
    QPushButton* SearchButton;
    QPushButton* ProfileButton;
    QLineEdit *searchBar;
    QLabel* chat;
    QVector<VChatWidget*> all_vchats; // tmp
public slots:
    void handleSearch();
    void handle_vchat_click(QString);
    void keyPressEvent(QKeyEvent* event);
    void handleSearchButton();
    void handleProfileButton(); //Tiko Part
signals:
    void vchat_clicked_from_main_pg(QString nickname);
    void profile_button_signal();
};

#endif // MAIN_PAGE_WINDOW_H
