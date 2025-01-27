#include "navigationprevornext.h"
#include <QPushButton>
#include <QDebug>

navigationPrevOrNext::navigationPrevOrNext(QWidget *parent) : QWidget(parent)
{
    m_prev = new QPushButton("Prev", this);
    m_next = new QPushButton("Next", this);

    m_prev->setGeometry(0, 100, 90, 40);
    m_next->setGeometry(20, 100, 90, 40);

    m_next->setStyleSheet("background-color: green;");

    m_prev->move(0, 60);
    m_next->move(100, 60);

    connect(m_prev, &QPushButton::clicked, this, &navigationPrevOrNext::onPrevClicked);
    connect(m_next, &QPushButton::clicked, this, &navigationPrevOrNext::onNextClicked);
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
