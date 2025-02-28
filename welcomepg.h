#ifndef WELCOMEPG_H
#define WELCOMEPG_H

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

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
    QLabel *welcome_label;
    QPushButton *signUp;
    QPushButton *signIn;
    QPushButton *about;
    QComboBox *languageComboBox;
    QPushButton *thememode;
    QString theme;
signals:
    void theme_changed(QString mode);
public slots:
    void handle_theme_changed_from_settings(QString mode);
private:
    QString englishText;
    QString armenianText;
    QString russianText;

private:
    void init();
    void conecting();

public:
    void setLanguage();
};
#endif // WELCOMEPG_H
