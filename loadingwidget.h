#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>

class LoadingWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoadingWidget(QWidget *parent = nullptr);
    ~LoadingWidget();

private:
    QLabel *loadingLabel;
    QMovie *movie;
};

#endif // LOADINGWIDGET_H
