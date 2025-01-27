#include "v_chat_widget.h"

VChatWidget::VChatWidget(QString name_text, QString nick_text, QWidget *parent)
    : QWidget(parent) {
    this->setFixedSize(300, 60);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    QFrame *container = new QFrame();
    container->setStyleSheet(
        // "background-color: black;"
        "border: 1px solid white;"
        "border-radius: 10px;"
        );
    container->setFixedSize(200, 60);
    container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);



    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);


    name = new QLabel(name_text);
    name->setStyleSheet("color: white; font-size: 12px; border: none;");
    name->setAlignment(Qt::AlignLeft);
    // name->setFixedWidth(50);
    name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    nick = new QLabel(nick_text);
    nick->setStyleSheet("color: white; font-size: 12px; border: 0px;");
    nick->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nick->setAlignment(Qt::AlignRight);

    pic = new QLabel();
    QPixmap avatarImage("C:\\Users\\aregm_\\Qt_projects\\MainPage\\images\\avatar.png");
    pic->setPixmap(avatarImage.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pic->setFixedSize(40, 40);
    pic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pic->setStyleSheet("border: 0px");

    layout->addWidget(name);
    layout->addWidget(pic);
    layout->addWidget(nick);

    name->update();

    nick->update();

    mainLayout->addWidget(container, 0, Qt::AlignCenter);
    setLayout(mainLayout);

    timer = nullptr;
    position = 0;

    set_user_pic("C:\\Users\\aregm_\\Pictures\\panda.jpg");
}


void VChatWidget::mousePressEvent(QMouseEvent *event) {
    qDebug() << "its worked";
    emit clicked_vchat();
}


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

void VChatWidget::set_user_pic(const QString &pic_path)
{
    QPixmap avatar(pic_path);

    if (avatar.isNull()) {
        qDebug() << "Failed to load image: " << pic_path;
        std::exit(0);
    }

    int size = 40;
    QPixmap triangle(size, size);
    triangle.fill(Qt::transparent);

    QPainter painter(&triangle);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.moveTo(size / 2, size);
    path.lineTo(0, 0);
    path.lineTo(size, 0);
    path.closeSubpath();

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, size, size, avatar);

    pic->setPixmap(triangle);
    pic->setFixedSize(size, size);
    pic->setStyleSheet("border: 0px;");
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
