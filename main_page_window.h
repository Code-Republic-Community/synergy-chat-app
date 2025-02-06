#ifndef MAIN_PAGE_WINDOW_H
#define MAIN_PAGE_WINDOW_H

#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QVector>
#include <QWidget>
#include "scroll_widget.h"
#include "v_chat_widget.h"
#include "httpclient.h"
#include <QStringList>

class MainPageWindow : public QWidget
{
    Q_OBJECT

public:
    MainPageWindow(QWidget *parent = nullptr);
    ~MainPageWindow() = default;
    void setLanguage();

private:
    ScrollWidget *scroll_widget;
    QPushButton *SearchButton;
    QPushButton *ProfileButton;
    QLineEdit *searchBar;
    QLabel *chat;
    QVector<VChatWidget *> all_vchats; // tmp
    HttpClient *client_main_page;
    QStringList contacts;
public slots:
    void handleSearch();
    void handle_vchat_click(QString);
    void keyPressEvent(QKeyEvent *event);
    void handleSearchButton();
    void handleProfileButton();
    void handleIdReceiving();
private slots:
    void handle_contact(QByteArray responseData);
    void fill_contacts();
signals:
    void vchat_clicked_from_main_pg(QString nickname);
    void profile_button_signal();
    void received_contacts();
};

#endif // MAIN_PAGE_WINDOW_H
