#ifndef LOADINGOVERLAY_H
#define LOADINGOVERLAY_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>

class LoadingOverlay : public QWidget {
    Q_OBJECT

public:
    explicit LoadingOverlay(QWidget *parent = nullptr);
    void showOverlay();
    void hideOverlay();

protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    QLabel *loadingLabel;
    QMovie *movie;

private:
    void centerLoadingGif();

};

#endif // LOADINGOVERLAY_H
