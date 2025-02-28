#include "v_chat_widget.h"

VChatWidget::VChatWidget(QString name_text, QString nick_text, QString surname_text,QPixmap profile_pohoto, QWidget *parent)
    : QPushButton(parent)
{
    contact_name = name_text;
    contact_nickname = nick_text;
    contact_surname = surname_text;
    contact_photo = profile_pohoto;
    this->setMinimumSize(200, 60);
    this->setMaximumSize(300, 60);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QFrame *container = new QFrame();
    container->setObjectName("frame_container");
    // container->setStyleSheet(
    //     "border: 3px solid #0078D7;"
    //     "border-radius: 10px;"
    //     "background-color: #EAEAEA");
    container->setFixedWidth(240);
    container->setFixedHeight(60);
    container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);

    name = new QLabel();
    // name->setStyleSheet("color: #0078D7; font-size: 12px; border: none;");
    name->setAlignment(Qt::AlignLeft);
    name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    nick = new QLabel();
    // nick->setStyleSheet("color: #0078D7; font-size: 12px; border: none;");
    nick->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nick->setAlignment(Qt::AlignRight);

    pic = new QLabel();
    pic->setObjectName("pic_vchat");
    pic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pic->setPixmap(cut_photo(contact_photo, 40));
    pic->setFixedSize(40, 40);

    layout->setSpacing(10);
    layout->addWidget(name);
    layout->addWidget(pic);
    layout->addWidget(nick);

    name->update();

    nick->update();

    mainLayout->addWidget(container, 0, Qt::AlignCenter);
    setLayout(mainLayout);

    // timer = nullptr;
    // position = 0;

    setLanguage(name_text, nick_text);
    connect(this, &QPushButton::clicked, this, [this](){
        emit clicked_vchat(contact_nickname, contact_name, contact_surname, contact_photo);
    });
}

// void VChatWidget::scroll_long_text(QString text)
// {
//     if (!timer) {
//         timer = new QTimer(this);
//         connect(timer, &QTimer::timeout, this, &VChatWidget::update_text);
//     }

//     position = 0;
//     scroll_text = text + "        ";
//     timer->start(400);
// }

// void VChatWidget::update_text()
// {
//     if (!name)
//         return;

//     int visibleText = qMin(10, scroll_text.length());
//     name->setText(scroll_text.mid(position, visibleText));
//     position++;

//     if (position >= scroll_text.length() - visibleText) {
//         position = 0;
//     }
// }

void VChatWidget::set_name(QString text)
{
    name->setText(text);
    contact_name = text;
}

QPixmap VChatWidget::cut_photo(QPixmap profile_photo, int size, QColor color)
{
    QPixmap avatar(profile_photo);

    if (avatar.isNull()) {
        qDebug() << "Failed to load image: ";
        avatar = QPixmap(":/pngs/panda");
    }

    double borderThickness = 2.2;

    QPixmap triangle(size, size);
    triangle.fill(Qt::transparent);

    QPainter painter(&triangle);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.moveTo(size / 2, size - borderThickness);
    path.lineTo(borderThickness, borderThickness);
    path.lineTo(size - borderThickness, borderThickness);
    path.closeSubpath();

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, size, size, avatar);

    painter.setClipping(false);
    QPen pen(color);
    pen.setWidth(borderThickness);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(path);

    return triangle;
}

void VChatWidget::set_nick(QString text)
{
    nick->setText("@" + text);
    contact_nickname = text;
}

QString VChatWidget::get_nick() const
{
    return contact_nickname;
}

QString VChatWidget::get_name() const
{
    return contact_name;
}

QString VChatWidget::get_surname() const
{
    return contact_surname;
}

void VChatWidget::setLanguage(QString name_text, QString nick_text)
{
    name->setText(name_text);
    nick->setText("@" + nick_text);
    pic->setToolTip(tr("User Profile Picture"));
}
