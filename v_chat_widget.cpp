#include "v_chat_widget.h"

VChatWidget::VChatWidget(QString name_text, QString nick_text, QWidget *parent)
    : QPushButton(parent) {
    // this->setFixedSize(300, 60);
    this->setMinimumSize(200, 60);
    this->setMaximumSize(300, 60);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    QFrame *container = new QFrame();
    container->setStyleSheet(
        // "background-color: black;"
        "border: 0px solid white;"
        "border-radius: 10px;"
        );
    container->setFixedSize(200, 60);
    container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);



    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);


    name = new QLabel();
    name->setStyleSheet("color: white; font-size: 12px; border: none;");
    name->setAlignment(Qt::AlignLeft);
    // name->setFixedWidth(50);
    name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    nick = new QLabel();
    nick->setStyleSheet("color: white; font-size: 12px; border: 0px;");
    nick->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nick->setAlignment(Qt::AlignRight);

    pic = new QLabel();
    QPixmap avatarImage("C:\\Users\\aregm_\\Qt_projects\\MainPage\\images\\avatar.png");
    pic->setPixmap(avatarImage.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pic->setFixedSize(40, 40);
    pic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pic->setStyleSheet("border: 0px");

    layout->setSpacing(10);
    layout->addWidget(name);
    layout->addWidget(pic);
    layout->addWidget(nick);

    name->update();

    nick->update();

    mainLayout->addWidget(container, 0, Qt::AlignCenter);
    setLayout(mainLayout);

    timer = nullptr;
    position = 0;

    pic->setPixmap(cut_photo(":/pngs/panda", 40));
    pic->setFixedSize(40, 40);
    pic->setStyleSheet("border: 0px;");

    setLanguage(name_text, nick_text);
    connect(this, &QPushButton::clicked, this, &VChatWidget::handle_click);
}


// void VChatWidget::mousePressEvent(QMouseEvent *event) {
//     qDebug() << "its worked";
//     emit clicked_vchat(this);
// }


void VChatWidget::scroll_long_text(QString text)
{
    if (!timer) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &VChatWidget::update_text);
    }

    position = 0;
    scroll_text = text + "        ";
    timer->start(400);
}

void VChatWidget::update_text()
{
    if (!name) return;

    int visibleText = qMin(10, scroll_text.length());
    name->setText(scroll_text.mid(position, visibleText));
    position++;

    if (position >= scroll_text.length() - visibleText) {
        position = 0;
    }
}

void VChatWidget::set_name(QString text)
{
    name->setText(text);
}

QPixmap VChatWidget::cut_photo(const QString &pic_path, int size)
{
    QPixmap avatar(pic_path);

    if (avatar.isNull()) {
        qDebug() << "Failed to load image: " << pic_path;
        std::exit(0);
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

    QPen pen(QColor("#8e15de"));
    pen.setWidth(borderThickness);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(path);

    return triangle;
}



void VChatWidget::handle_click()
{
    emit clicked_vchat(this->get_nick());
}


void VChatWidget::set_nick(QString text)
{
    nick->setText(text);
}


QString VChatWidget::get_nick() const {
    return this->nick->text();
}


QString VChatWidget::get_name() const {
    return this->name->text();
}

void VChatWidget::setLanguage(QString name_text, QString nick_text)
{
    name->setText(name_text);
    nick->setText(nick_text);
    pic->setToolTip(tr("User Profile Picture"));
}
