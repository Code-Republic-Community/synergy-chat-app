#ifndef REGISTRATION_h
#define REGISTRATION_h

//#include "navigationprevornext.h"
#include <QMainWindow>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QDateEdit>
#include <QVector>
#include <QLabel>
#include "httpclient.h"

class Registration : public QWidget
{
    Q_OBJECT

public:
    Registration(QWidget *parent = nullptr);
    void init();
    ~Registration();

    bool checkFormValidity();
private:
    //navigationPrevOrNext *m_nextAndPrev;
    QWidget *centralWidget;
    QVBoxLayout* mainLayout;
    QVBoxLayout* topLayout;
    QHBoxLayout* middleLayout;
    QHBoxLayout* bottomLayout;
    //QHBoxLayout* rowLayout;
    QDateEdit* dateEdit;
    QFormLayout* formLayout;
    QLineEdit* nameField;
    QLineEdit* surnameField;
    QLineEdit* nicknameField;
    QLineEdit* emailField;
    QLineEdit* passwordField;
    QLineEdit* confirmPasswordField;
    QLabel* nameLabel;
    QLabel* surnameLabel;
    QLabel* nicknameLabel;
    QLabel* emailLabel;
    QLabel* dateLabel;
    QLabel* passwordLabel;
    QLabel* confirmPasswordLabel;
    QCheckBox* checkBox;
    QPushButton* termsOfUseButton;
    QPushButton* haveAccountButton;
    QPushButton* prevButton;
    QPushButton* registerButton;
    QLabel* topLabel;
    QLabel* smallText;
    HttpClient *client_login;
    QString name_txt;
    QString nickname_txt;
    QString surname_txt;
    QString date_txt;
    QString email_txt;
    QString password_txt;

signals:
    void terms_of_use_signal();
    void have_an_account_signal();
    void reg_btn_signal();
    void prev_btn_signal();

private slots:

    void handle_terms_of_use_btn();
    void handle_have_an_account_btn();
    void handle_reg_btn();
    void handle_prev_btn();
private:
    void save_texts();
};

#endif // REGISTRATION_h
