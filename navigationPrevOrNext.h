#ifndef NAVIGATIONPREVORNEXT_H
#define NAVIGATIONPREVORNEXT_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class navigationPrevOrNext : public QWidget
{
    Q_OBJECT

public:
    navigationPrevOrNext(QWidget *parent = nullptr);

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
