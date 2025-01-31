#ifndef V_CHAT_WIDGET_H
#define V_CHAT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QFrame>
#include <QString>
#include <QTimer>
#include <QPainterPath>
#include <QBitmap>
#include <QPainter>
#include <QPushButton>

class VChatWidget : public QPushButton {
    Q_OBJECT
public:
    VChatWidget(QString name_text, QString nick_text, QWidget *parent = nullptr);
    ~VChatWidget() = default;

signals:
    void clicked_vchat(QString nick);

protected:
    // void mousePressEvent(QMouseEvent *event) override;

public:
    void scroll_long_text(QString text);
    void update_text();

private:
    QLabel *pic;
    QLabel *name;
    QLabel *nick;

public:
    QString get_nick() const;
    QString get_name() const;

    void set_nick(QString text);
    void set_name(QString text);
    void set_user_pic(const QString &pic_path);
    QPixmap cut_photo(const QString &pic_path);
private:
    QTimer *timer;
    int position;
    QString scroll_text;

private slots:
    void handle_click();
};

#endif // V_CHAT_WIDGET_H
