#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <QByteArray>
#include <QDebug>
#include <QEvent>
#include <QIntValidator>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTranslator>
#include <QVBoxLayout>
#include <QWidget>
#include "httpclient.h"

class Verification : public QWidget
{
    Q_OBJECT

public:
    Verification(QWidget *parent = nullptr);
    ~Verification();
    void clear_fields();
signals:
    void nextClicked();
    void prevClicked();
    void verification_successfull();
private slots:
    void onPrevClicked();
    void onNextClicked();
    void handle_data(QByteArray responseData);
public slots:
    void handleEmail(QString email);

private:
    QString maskEmail(const QString &email);

private:
    inline static int chanceleft = 3;
    QLabel *verificationtxt;
    QLineEdit *code;
    QLabel *chance;
    QPushButton *Back;
    QPushButton *Next;
    QString original_code;
    HttpClient *client_verification;
    QString maskedemail;

public:
    void setLanguege();
};

#endif // VERIFICATION_H
