#include "loadingoverlay.h"

#include <QGraphicsBlurEffect>
#include <QResizeEvent>

LoadingOverlay::LoadingOverlay(QWidget *parent) : QWidget(parent) {
    setParent(parent);
    setFixedSize(parent->size());

    setStyleSheet("background-color: rgba(0, 0, 0, 50);");
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_NoSystemBackground, true);

    loadingLabel = new QLabel(this);
    movie = new QMovie(":/pngs/load.gif");

    movie->setScaledSize(QSize(70, 70));

    loadingLabel->setMovie(movie);
    loadingLabel->setAlignment(Qt::AlignCenter);
    movie->start();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(loadingLabel);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    hide();
}

void LoadingOverlay::showOverlay() {
    centerLoadingGif();
    raise();
    show();
}

void LoadingOverlay::hideOverlay() {
    hide();
}

void LoadingOverlay::centerLoadingGif() {
    if (!loadingLabel) return;

    QWidget *parentWidget = static_cast<QWidget*>(parent());
    if (!parentWidget) return;

    int x = (parentWidget->width() - loadingLabel->width()) / 2;
    int y = (parentWidget->height() - loadingLabel->height()) / 2;
    loadingLabel->move(x, y);
}

void LoadingOverlay::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    centerLoadingGif();
}
