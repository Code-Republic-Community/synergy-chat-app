#include "navigationPrevOrNext.h"

#include <QDebug>
#include <QPushButton>

navigationPrevOrNext::navigationPrevOrNext(QWidget *parent)
    : QWidget(parent)
{
    m_prev = new QPushButton(this);
    m_next = new QPushButton(this);

    m_prev->setGeometry(0, 0, 100, 40);
    m_next->setGeometry(110, 0, 100, 40);


    connect(m_prev, &QPushButton::clicked, this, &navigationPrevOrNext::onPrevClicked);
    connect(m_next, &QPushButton::clicked, this, &navigationPrevOrNext::onNextClicked);
}

void navigationPrevOrNext::setLeftButton(QString st)
{
    m_prev->setText(st);
}

void navigationPrevOrNext::setRightButton(QString st)
{
    m_next->setText(st);
}

void navigationPrevOrNext::onPrevClicked()
{
    qDebug() << "Previous button clicked!";
    emit prevClicked();
}

void navigationPrevOrNext::onNextClicked()
{
    qDebug() << "Next button clicked!";
    emit nextClicked();
}
