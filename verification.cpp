#include "verification.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include "globals.h"

Verification::Verification(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(400, 700);
    client_verification = new HttpClient();
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    verificationtxt = new QLabel(this);
    verificationtxt->setWordWrap(true);
    verificationtxt->setStyleSheet("font-size: 17px; font-weight: bold;");
    verificationtxt->setAlignment(Qt::AlignCenter);
    layout->addWidget(verificationtxt, 0, Qt::AlignTop | Qt::AlignHCenter);

    code = new QLineEdit(this);
    code->setFixedSize(200, 50);
    code->setStyleSheet("font-size: 16px; padding: 10px;");
    code->setValidator(new QIntValidator(100000, 999999, this));
    code->setMaxLength(7);
    layout->addWidget(code, 1, Qt::AlignHCenter);

    code->setMaxLength(7);
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

    chance = new QLabel(this);
    chance->setStyleSheet("font-size: 12px; color: red;");
    chance->setAlignment(Qt::AlignCenter);
    layout->addWidget(chance);

    Back = new QPushButton(this);
    Next = new QPushButton(this);
    Back->setGeometry(20, 600, 90, 40);
    Next->setGeometry(300, 600, 90, 40);
    Next->setStyleSheet("background-color: green;");

    connect(Back, &QPushButton::clicked, this, &Verification::onPrevClicked);
    connect(Next, &QPushButton::clicked, this, &Verification::onNextClicked);
    setLanguege();
    setLayout(layout);
}

void Verification::setLanguege()
{
    verificationtxt->setText(tr("Verification code sent to your Email: ") + maskedemail);
    code->setPlaceholderText(tr("Enter the 6-digit code"));
    code->setAlignment(Qt::AlignCenter);
    chance->setText(tr("You have 3 chances"));
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
    connect(client_verification, &HttpClient::responseReceived, this, &Verification::handle_data);
    QUrl url("https://synergy-iauu.onrender.com/verify/");
    QJsonObject jsonData;
    jsonData["user_id"] = Globals::getInstance().getUserId();
    jsonData["verification_code"] = original_code.toInt();

    client_verification->postRequest(url, jsonData);
}

void Verification::handle_data(QByteArray responseData)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();
    if (chanceleft <= 0) {
        chance->setText(tr("You have") + QString::number(chanceleft) + tr("chances"));
        emit prevClicked();
    } else {
        if (jsonObject.contains("message")
            && jsonResponse["message"].toString() == "Verification successful") {
            qDebug() << "Verification successful!";
            emit nextClicked();
        } else if (jsonObject.contains("message")
                   && jsonResponse["message"].toString() == "Invalid verification code") {
            --chanceleft;
            qDebug() << "Invalid verification code";
            chance->setText(tr("You have") + QString::number(chanceleft) + tr("chances"));
            clear_fields();
        } else {
            --chanceleft;
            qDebug() << "NEMA";
            chance->setText(tr("You have") + QString::number(chanceleft) + tr("chances"));
            clear_fields();
        }
    }
}

Verification::~Verification()
{
    delete verificationtxt;
    delete code;
    delete chance;
    delete Back;
    delete Next;
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
    chanceleft = 3;
    maskedemail = maskEmail(email);
    qDebug() << "Masked Email: " << maskedemail;
    setLanguege();
}
