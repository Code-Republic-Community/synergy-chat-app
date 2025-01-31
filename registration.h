// #ifndef REGISTRATION_h
// #define REGISTRATION_h

// #include "navigationPrevOrNext.h"
// #include <QMainWindow>
// #include <QPushButton>
// #include <QFormLayout>
// #include <QHBoxLayout>
// #include <QVBoxLayout>
// #include <QLineEdit>
// #include <QCheckBox>
// #include <QDateEdit>
// #include <QVector>
// #include <QLabel>


// class Registration : public QWidget
// {
//     Q_OBJECT

// public:
//     Registration(QWidget *parent = nullptr);
//     void init();
//     ~Registration();

//     bool checkFormValidity();
// private:
//     //navigationPrevOrNext *m_nextAndPrev;
//     QWidget *centralWidget;
//     QVBoxLayout* mainlayout;
//     QVBoxLayout* toplayout;
//     QHBoxLayout* middlelayout;
//     QHBoxLayout* bottomlayout;
//     QDateEdit* dateEdit;
//     QFormLayout* formlayout;
//     QLineEdit* nameField;
//     QLineEdit* surnameField;
//     QLineEdit* nicknameField;
//     QLineEdit* emailField;
//     QLineEdit* passwordField;
//     QLineEdit* confirmPasswordField;
//     QCheckBox* checkbox;
//     QPushButton* termsOfUseButton;
//     QPushButton* haveAccountButton;
//     //QPushButton* loginButton;
//     QPushButton* prevButton;
//     QPushButton* registerButton;
//     QLabel* toplabel;
//     QLabel* smalltext;



// signals:
//     void terms_of_use_signal();
//     void have_an_account_signal();
//     void reg_btn_signal();
//     void prev_btn_signal();

// private slots:

//     void handle_terms_of_use_btn();
//     void handle_have_an_account_btn();
//     void handle_reg_btn();
//     void handle_prev_btn();
// };


// #endif // REGISTRATION_h


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
    QCheckBox* checkBox;
    QPushButton* termsOfUseButton;
    QPushButton* haveAccountButton;
    QPushButton* prevButton;
    QPushButton* registerButton;
    QLabel* topLabel;
    QLabel* smallText;

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
};

#endif // REGISTRATION_h

