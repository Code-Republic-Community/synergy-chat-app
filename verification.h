#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QTranslator>
#include "httpclient.h"
#include <QByteArray>

class Verification : public QWidget
{
    Q_OBJECT

public:
    Verification(QWidget *parent = nullptr);
    ~Verification();
signals:
    void nextClicked();
    void prevClicked();
    void verification_successfull();
private slots:
    void onPrevClicked();
    void onNextClicked();
    void handle_data(QByteArray responseData);
private:
    QLabel *verificationtxt;
    QLineEdit *code;
    QLabel *chance;
    QPushButton *Back;
    QPushButton *Next;
    QString original_code;
    //navigationPrevOrNext *m_nextAndPrev;
    int chanceCnt = 3;
    HttpClient *client_verification;
public:
    void setLanguege();
    // void setTextForElements(const QString &verificationMessage, const QString &placeholder, const QString &chanceMessage, const QString &VerMss, const QString &BackMs);
};

#endif // VERIFICATION_H
