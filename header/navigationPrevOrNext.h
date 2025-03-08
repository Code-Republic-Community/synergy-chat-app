#ifndef NAVIGATIONPREVORNEXT_H
#define NAVIGATIONPREVORNEXT_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class navigationPrevOrNext : public QWidget
{
    Q_OBJECT

public:
    navigationPrevOrNext(QWidget *parent = nullptr);
    void setLeftButton(QString st);
    void setRightButton(QString st);

signals:
    void nextClicked();
    void prevClicked();

private:
    QPushButton *m_prev;
    QPushButton *m_next;
    QHBoxLayout *m_layout;

private slots:
    void onPrevClicked();
    void onNextClicked();
};

#endif // NAVIGATIONPREVORNEXT_H
