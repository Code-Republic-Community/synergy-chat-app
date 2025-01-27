#ifndef REGISTRATION_h
#define REGISTRATION_h

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
    ~Registration();

private:
    QWidget *centralWidget;
    QVBoxLayout* mainlayout;
    QHBoxLayout* toplayout;
    QHBoxLayout* middlelayout;
    QHBoxLayout* bottomlayout;
    QDateEdit* dateEdit;
    QFormLayout* formlayout;
    QLineEdit* nameField;
    QLineEdit* surnameField;
    QLineEdit* nicknameField;
    QLineEdit* emailField;
    QLineEdit* passwordField;
    QLineEdit* confirmPasswordField;
    QCheckBox* checkbox;
    QPushButton* loginbutton;
    QPushButton* prevbutton;
    QPushButton* registerbutton;
    QLabel* toplabel;
    QLabel* toptext;
    QLabel* bottomlabel;

    /*public slots:
    void nameChanged(const QString &);
    void surnameChanged(const QString &);
    void nicknameChanged(const QString &);
    void emailChanged(const QString &);
    void dobChanged(const QString &);
    void passwordChanged(const QString &);
    void confirmpasswordChanged(const QString &);*/

signals:
    void reg_btn_signal();
    void prev_btn_signal();

private slots:
    void handle_reg_btn();
    void handle_prev_btn();
};


#endif // REGISTRATION_h
