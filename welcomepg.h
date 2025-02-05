#ifndef WELCOMEPG_H
#define WELCOMEPG_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QDialog>
#include <QComboBox>

class WelcomePg : public QWidget
{
    Q_OBJECT

public:
    WelcomePg(QWidget *parent = nullptr);
    ~WelcomePg();
signals:
    void signInClicked();
    void signUpClicked();
    void languageChanged();
private slots:
    void onSignInClicked();
    void onSignUpClicked();
private:
    QLabel* welcome_label;
    QLabel* text_tras;
    QPushButton* signUp;
    QPushButton* signIn;
    QPushButton* about;
    QComboBox* languageComboBox;
private:
    void init();
    void conecting();
public:
    void setLanguage();
};
#endif // WELCOMEPG_H
