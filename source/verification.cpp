#include "verification.h"
#include "globals.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QUrl>

Verification::Verification(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    client_verification = new HttpClient();
    overlay = new LoadingOverlay(this);

    QWidget *toplayoutContainer = new QWidget(this);
    toplayoutContainer->setGeometry(0, 0, 400, 200);
    QHBoxLayout *toplayout = new QHBoxLayout(toplayoutContainer);
    toplayout->setContentsMargins(50, 40, 50, 20);

    verificationtxt = new QLabel(this);
    verificationtxt->setObjectName("verificationtxt");
    verificationtxt->setWordWrap(true);
    verificationtxt->setAlignment(Qt::AlignCenter);
    toplayout->addWidget(verificationtxt, 0, Qt::AlignTop | Qt::AlignHCenter);

    QWidget *midlayoutContainer = new QWidget(this);
    midlayoutContainer->setGeometry(0, 300, 400, 100);
    QVBoxLayout *midlayout = new QVBoxLayout(midlayoutContainer);
    midlayout->setContentsMargins(50, 0, 50, 0);

    code = new QLineEdit(this);
    code->setObjectName("code");
    code->setFixedSize(200, 50);
    code->setValidator(new QIntValidator(100000, 999999, this));
    code->setMaxLength(7);
    midlayout->addWidget(code, 0, Qt::AlignHCenter);

    chance = new QLabel;
    chance->setAlignment(Qt::AlignCenter);
    midlayout->addWidget(chance, 0, Qt::AlignHCenter);

    QWidget *bottomLayoutContainer = new QWidget(this);
    bottomLayoutContainer->setGeometry(0, 600, 400, 100);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomLayoutContainer);
    Back = new QPushButton(this);
    Next = new QPushButton(this);
    Back->setMinimumWidth(100);
    Next->setMinimumWidth(100);

    bottomLayout->setAlignment(Qt::AlignCenter);
    bottomLayout->addStretch();
    bottomLayout->addWidget(Back);
    bottomLayout->addItem(new QSpacerItem(20, 0));
    bottomLayout->addWidget(Next);
    bottomLayout->addStretch();

    connect(QApplication::instance(), &QApplication::aboutToQuit, this, [=]() {
        delete this;
    });
    connect(Back, &QPushButton::clicked, this, &Verification::onPrevClicked);
    connect(Next, &QPushButton::clicked, this, &Verification::onNextClicked);
    setLanguege();

    connect(code, &QLineEdit::textEdited, this, [this](const QString &text) {
        original_code = text;
        static QString currentText;
        QString Text = text;
        Text.remove(QRegularExpression("[^\\d]"));
        QString formattedText = Text;
        if (Text.length() > 2) {
            formattedText = Text.left(3) + "-" + Text.mid(3);
        }
        if (!currentText.isEmpty() && text.length() < currentText.length()) {
            if (currentText.length() > 3 && currentText[3] == '-' && text.length() == 3) {
                formattedText.remove(2, 2);
            }
        }
        if (formattedText != text) {
            code->setText(formattedText);
        }
        currentText = code->text();
    });

}

void Verification::setLanguege()
{
    verificationtxt->setText(tr("Verification code sent to your Email: ") + maskedemail);
    code->setPlaceholderText(tr("Enter the 6-digit code"));
    code->setAlignment(Qt::AlignCenter);
    chance->setText(tr("You have") + " " + QString::number(chanceleft + 1)+ " " + tr("chances"));
    Next->setText(tr("Verify"));
    Back->setText(tr("Back"));
}

void Verification::onPrevClicked()
{
    qDebug() << "Previous button clicked!";
    Next->setEnabled(true);
    emit prevClicked();
}

void Verification::onNextClicked()
{
    overlay->showOverlay();
    connect(client_verification, &HttpClient::responseReceived, this, &Verification::handle_data);
    QUrl url(Globals::getInstance().getSERVER_LINK() + "verify/");
    QJsonObject jsonData;
    jsonData["user_id"] = Globals::getInstance().getUserId();
    jsonData["verification_code"] = original_code.toInt();
    client_verification->postRequest(url, jsonData);
}

void Verification::handle_data(QByteArray responseData)
{
    disconnect(client_verification, &HttpClient::responseReceived, this, &Verification::handle_data);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();
    if (chanceleft <= 0) {
        overlay->hideOverlay();
        chance->setText(tr("You have") + QString::number(chanceleft + 1) + tr("chances"));
        Next->setEnabled(true);
        chanceleft = 2;
        emit prevClicked();
    }
    else {
        if (jsonObject.contains("message") && jsonResponse["message"].toString() == "Verification successful") {
            qDebug() << "Verification successful";
            chanceleft = 2;
            overlay->hideOverlay();
            emit verification_successfull();
            emit nextClicked();
        }
        else if (jsonObject.contains("detail") && jsonResponse["detail"].toString() == "Invalid verification code") {
            --chanceleft;
            qDebug() << "Invalid verification code";
            chance->setText(tr("You have") + " " + QString::number(chanceleft + 1)+ " " + tr("chances"));
            overlay->hideOverlay();
            clear_fields();
        }
        else if (jsonObject.contains("detail") && jsonResponse["detail"].toString() == "Invalid or expired verification request"){
            qDebug() << "User id not found // Invalid or expired verification request";
            chanceleft = 2;
            overlay->hideOverlay();
            clear_fields();
            emit prevClicked();
        }
    }
}

Verification::~Verification()
{
}

void Verification::clear_fields()
{
    code->setText("");
}

QString Verification::maskEmail(const QString &email)
{

    if (email.isEmpty()) {
        return "Invalid Email";
    }

    int atIndex = 0;

    for (int i = 0; i < email.length(); ++i) {
        if (email[i] == "@") {
            atIndex = i;
            break;
        }
    }
    if (atIndex <= 1) {
        return email;
    }
    QString masked = email;
    for (int i = 1; i < atIndex - 1; ++i) {
        masked[i] = '*';
    }
    return masked;
}

void Verification::handleEmail(QString email)
{
    chanceleft = 2;
    maskedemail = maskEmail(email);
    qDebug() << "Masked Email: " << maskedemail;
    setLanguege();
}
