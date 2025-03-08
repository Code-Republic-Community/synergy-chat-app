#include "loadingwidget.h"

LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(400, 700);

    loadingLabel = new QLabel(this);
    movie = new QMovie(":/pngs/resources/load.gif");

    movie->setScaledSize(QSize(100, 100));

    loadingLabel->setMovie(movie);
    loadingLabel->setObjectName("loadingLabel");
    loadingLabel->setAlignment(Qt::AlignCenter);
    movie->start();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(loadingLabel);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);
}

LoadingWidget::~LoadingWidget() {
    delete movie;
    delete loadingLabel;
}
