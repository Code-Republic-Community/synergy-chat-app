#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "chat_widget.h"
#include "login.h"
#include "main_page_window.h"
#include "my_profile.h"
#include "otherprofile.h"
#include "registration.h"
#include "settings.h"
#include "verification.h"
#include "welcomepg.h"
#include "loadingwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
public slots:
    void changeTheme(QString mode);
signals:
    void theme_changed(QString mode);
private:
    QStackedWidget *staked_widget;
    WelcomePg *welcome_pg;
    Login *login_pg;
    Registration *reg_pg;
    MainPageWindow *main_pg;
    ChatWidget *chat_pg;
    MyProfile *profile_settings_pg;
    Settings *settings_pg;
    Verification *verification_pg;
    OtherProfile *other_profile_pg;
    LoadingWidget *loading_page;

private:
    void openSavedAccount();


private slots:
    void goToSignIn();
    void goToRegPg();
    void goToWelcomePg();
    void goToMainPg();
    void goToChatPg();
    void goToProfileSettingsPg();
    void goToSettings();
    void goToVerificationPg();
    void goToOtherProfilePg();
    void change_language();
    void goto_loading_page();
};
#endif // MAINWINDOW_H
