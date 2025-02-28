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
    VChatWidget(QString name_text,
                QString nick_text,
                QString surname_text,
                QPixmap profile_pohoto,
                QWidget *parent = nullptr);
    ~VChatWidget() = default;

signals:
    void clicked_vchat(QString nickname, QString name, QString surname, QPixmap photo);

// public:
//     void scroll_long_text(QString text);
//     void update_text();

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
    QPixmap contact_photo;

public:
    QString get_nick() const;
    QString get_name() const;
    QString get_surname() const;

    void set_nick(QString text);
    void set_name(QString text);
    void set_surname(QString text);
    void set_photo(QPixmap profile_photo);
    static QPixmap cut_photo(QPixmap profile_photo, int size, QColor color = QColor("#0078D7"));

private:
    QTimer *timer;
    int position;
    QString scroll_text;

};

#endif // V_CHAT_WIDGET_H
