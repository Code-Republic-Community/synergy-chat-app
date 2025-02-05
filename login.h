#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QMouseEvent>
#include <QTimer>
#include "navigationPrevOrNext.h"//
#include "httpclient.h"
#include "globals.h"
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    void init();
    void setLanguage();
    ~Login();

private:
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
    QLabel *m_label1;
    QLabel *m_label2;
    HttpClient *client_login;
    QString m_usernameText;
    QString m_passwordText;

signals:
    void prev_btn_signal();
    void next_btn_signal();
    void register_signal();
    void idreceived();

private slots:
    void rememberMe(bool isClicked);
    void mousePressEvent(QMouseEvent* event);
    void handleNextButtonClicked();
    void handlePrevButtonClicked();
    void forgetIcon();
    void saveTexts();
    void handleUserId(QByteArray responseData);
};

#endif // LOGIN_H
