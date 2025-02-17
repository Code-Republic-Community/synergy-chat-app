#ifndef LOGIN_H
#define LOGIN_H

#include <QCheckBox>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include "httpclient.h"
#include "navigationPrevOrNext.h"

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    void init();
    void setLanguage();
    void clear_fields();
    ~Login();

private:
    bool remember;
    QLabel *m_loginLabel;
    QFont m_fontForLogin;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLabel *statusLabel;
    QLabel *m_usernameLabel;
    QLabel *m_passwordLabel;
    QLabel *m_underLoginText;
    QCheckBox *m_rememberMe;
    QLabel *m_dontHaveAnAccount;
    QFont m_fontForHaventAccount;
    QLabel *m_register;
    navigationPrevOrNext *m_nextAndPrev;
    QLabel *m_forget;

    HttpClient *client_login;
    QString m_usernameText;
    QString m_passwordText;

private:
    void saveCredentials(const QString &userId, const QString &username, const QString &password);

    void clearCredentials();

public slots:
    void loadCredentials();

signals:
    void prev_btn_signal();
    void next_btn_signal();
    void register_signal();
    void idreceived();
    void startloading();
    void stoploading();

private slots:
    void rememberMe(bool isClicked);
    void mousePressEvent(QMouseEvent *event);
    void handleNextButtonClicked();
    void handlePrevButtonClicked();
    void saveTexts();
    void handleUserId(QByteArray responseData);
    //load account login password
};

#endif // LOGIN_H
