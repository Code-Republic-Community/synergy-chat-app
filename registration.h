#ifndef REGISTRATION_h
#define REGISTRATION_h

//#include "navigationprevornext.h"
#include <QCheckBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
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
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QVBoxLayout *topLayout;
    QHBoxLayout *middleLayout;
    QHBoxLayout *bottomLayout;

    QDateEdit *dateEdit;
    QFormLayout *formLayout;
    QLineEdit *nameField;
    QLineEdit *surnameField;
    QLineEdit *nicknameField;
    QLineEdit *emailField;
    QLineEdit *passwordField;
    QLineEdit *confirmPasswordField;
    QLabel *nameLabel;
    QLabel *surnameLabel;
    QLabel *nicknameLabel;
    QLabel *emailLabel;
    QLabel *dateLabel;
    QLabel *passwordLabel;
    QLabel *confirmPasswordLabel;
    QCheckBox *checkBox;
    QPushButton *termsOfUseButton;
    QPushButton *haveAccountButton;
    QPushButton *prevButton;
    QPushButton *registerButton;
    QLabel *topLabel;
    HttpClient *client_registration;
    QString name_txt;
    QString nickname_txt;
    QString surname_txt;
    QString date_txt;
    QString email_txt;
    QString password_txt;
    bool valids[8];

public:
    void setLanguage();
signals:
    void terms_of_use_signal();
    void have_an_account_signal();
    void reg_btn_signal();
    void prev_btn_signal();
    void idreceived();

private slots:
    void handle_terms_of_use_btn();
    void handle_have_an_account_btn();
    void handle_reg_btn();
    void handle_prev_btn();
    void handleUserId(QByteArray responseData);

private:
    void save_texts();
};

#endif // REGISTRATION_h
