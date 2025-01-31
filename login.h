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
    QLabel *m_imageLabel1;
    QLabel *m_imageLabel2;

signals:
    void prev_btn_signal();
    void next_btn_signal();
    void register_signal();

private slots:
    void rememberMe(bool isClicked);
    void mousePressEvent(QMouseEvent* event);
    void handleNextButtonClicked();
    void handlePrevButtonClicked();
    void forgetIcon();
};

#endif // LOGIN_H
