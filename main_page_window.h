#ifndef MAIN_PAGE_WINDOW_H
#define MAIN_PAGE_WINDOW_H

#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QStringList>
#include <QVector>
#include <QWidget>

#include "httpclient.h"
#include "scroll_widget.h"
#include "v_chat_widget.h"
#include "loadingoverlay.h"

class MainPageWindow : public QWidget
{
    Q_OBJECT

public:
    MainPageWindow(QWidget *parent = nullptr);
    ~MainPageWindow() = default;
    void setLanguage();

private:
    void connections();

private:
    ScrollWidget *scroll_widget;
    QPushButton *SearchButton;
    QPushButton *ProfileButton;
    QLineEdit *searchBar;
    QLabel *chat;
    HttpClient *client_main_page;

    QVector<VChatWidget *> matched_contacts;
    QVector<VChatWidget *> matched_other_users;

private:
    QVector<VChatWidget *> contacts;
    QStringList contacts_nicknames_to_get_account_info;
private slots:
    void handle_contact(QByteArray responseData);
    void get_contacts_info_and_show();
public slots:
    void handleIdReceiving();
public slots:
    void clearDataOnLogout();
    void handleContactReDonwnload();

private:
    void clear_matched_arrays();
    void clear_contact_array();

private slots:
    void handleSearch();
    void handle_search_data(QByteArray responseData);
signals:
    void found_users_by_search(bool yes_or_not);
    void contact_successfully_added_to_scrollWidget();
    void matched_contact_or_other_user_added_successfully(bool contact_or_otherUser);
private slots:
    void vchat_clicked_from_search_pg(QString nickname, QString name, QString surname, QPixmap photo);

public slots:
    void handle_vchat_click(QString nickname, QString name, QString surname, QPixmap photo);
    void handleProfileButton();

signals:
    void vchat_clicked_from_main_pg(QString nickname, QString name, QString surname, QPixmap photo);
    void profile_button_signal();
    void received_contacts();

public slots:
    void handle_profile_photo_change(QPixmap photo);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    QPixmap profile_picture;
private:
    LoadingOverlay *overlay;
};

#endif // MAIN_PAGE_WINDOW_H
