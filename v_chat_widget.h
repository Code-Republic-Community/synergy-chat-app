#ifndef V_CHAT_WIDGET_H
#define V_CHAT_WIDGET_H

#include <QBitmap>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QWidget>

class VChatWidget : public QPushButton
{
    Q_OBJECT
public:
    VChatWidget(QString name_text, QString nick_text, QString surname_text, QWidget *parent = nullptr);
    ~VChatWidget() = default;

signals:
    void clicked_vchat(QString nick);

protected:
    // void mousePressEvent(QMouseEvent *event) override;

public:
    void scroll_long_text(QString text);
    void update_text();

private:
    void setLanguage(QString name_text, QString nick_text);

private:
    QLabel *pic;
    QLabel *name;
    QLabel *nick;
private:
    QString contact_nickname;
    QString contact_name;
    QString contact_surname;

public:
    QString get_nick() const;
    QString get_name() const;
    QString get_surname() const;

    void set_nick(QString text);
    void set_name(QString text);
    void set_user_pic(const QString &pic_path);
    static QPixmap cut_photo(const QString &pic_path, int size);

private:
    QTimer *timer;
    int position;
    QString scroll_text;

private slots:
    void handle_click();
};

#endif // V_CHAT_WIDGET_H
