#ifndef WELCOMEPG_H
#define WELCOMEPG_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QDialog>
#include "registration.h"

class WelcomePg : public QMainWindow
{
    Q_OBJECT

public:
    WelcomePg(QWidget *parent = nullptr);
    ~WelcomePg();
signals:
    void signInClicked();
    void signUpClicked();
private slots:
    void onSignInClicked();
    void onSignUpClicked();
private:
    QLabel* welcome_label;
    QLabel* text_tras;
    QPushButton* signUp;
    QPushButton* signIn;
    QPushButton* webPg;
    QPushButton* about;
private:
    void init();
    void conecting();
    void setLaguage();
};
#endif // WELCOMEPG_H
