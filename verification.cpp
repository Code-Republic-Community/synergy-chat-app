#include "verification.h"
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
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
    verificationtxt->setStyleSheet("font-size: 25px; font-weight: bold;");
    verificationtxt->setAlignment(Qt::AlignCenter);
    layout->addWidget(verificationtxt, 0, Qt::AlignTop | Qt::AlignHCenter);
    //layout->addWidget(verificationtxt);

    code = new QLineEdit(this);
    code->setFixedSize(200, 50);
    code->setStyleSheet("font-size: 16px; padding: 10px;");
    code->setValidator( new QIntValidator(100000, 999999, this));
    code->setMaxLength(7);
    layout->addWidget(code, 1, Qt::AlignHCenter);

    //QString currentText;

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
    //layout->addWidget(chance, 2, Qt::AlignCenter);


    // m_nextAndPrev = new navigationPrevOrNext(this);
    // //layout->addWidget(m_nextAndPrev, 0, Qt::AlignBottom | Qt::AlignCenter);
    // m_nextAndPrev->navigationPrevOrNext::setLeftButton("Back");
    // m_nextAndPrev->navigationPrevOrNext::setRightButton("Verify");
    // m_nextAndPrev->setGeometry(175, 565, 400, 100);


    // connect(m_nextAndPrev, &navigationPrevOrNext::nextClicked, this, [this]() {
    //     QString enteredCode = code->text();
    //     //  int chanceCnt = 3;
    //     QString correctCode = "123-456";
    //     if (enteredCode == correctCode) {
    //         qDebug() << "Code is correct!";
    //         m_nextAndPrev->setEnabled(true);
    //     } else {
    //         chanceCnt--;
    //         if (chanceCnt > 0) {
    //             chance->setText("You have " + QString::number(chanceCnt) + " chance");
    //         } else {
    //             chance->setText("No more chances!");
    //             m_nextAndPrev->setEnabled(false);
    //         }
    //         qDebug() << "Incorrect code " << chanceCnt;
    //     }

    // } );

    // //connect(m_nextAndPrev, &navigationPrevOrNext::prevClicked, this, &Verification::onPrevClicked);
    // //  connect(Next, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    // //connect(m_nextAndPrev, &navigationPrevOrNext::nextClicked, this, &Verification::onNextClicked);
    // connect(m_nextAndPrev, &navigationPrevOrNext::prevClicked, this, &Verification::onPrevClicked);



    Back = new QPushButton(this);
    Next = new QPushButton(this);
    Back->setGeometry(20, 600, 90, 40);
    Next->setGeometry(300, 600, 90, 40);
    Next->setStyleSheet("background-color: green;");
    //Next->setEnabled(false);
    //    connect(Next, &QPushButton::clicked, this, [this]() {
    //        QString enteredCode = code->text();
    //        //  int chanceCnt = 3;
    //        QString correctCode = "123-456";
    //        if (enteredCode == correctCode) {
    //            qDebug() << "Code is correct!";
    //            Next->setEnabled(true);
    //        } else {
    //            chanceCnt--;
    //            if (chanceCnt > 0) {
    //                chance->setText("You have " + QString::number(chanceCnt) + " chance");
    //            } else {
    //                chance->setText("No more chances!");
    //                Next->setEnabled(false);
    //            }
    //            qDebug() << "Incorrect code " << chanceCnt;
    //        }

    //    } );

    connect(Back, &QPushButton::clicked, this, &Verification::onPrevClicked);
    connect(Next, &QPushButton::clicked, this, &Verification::onNextClicked);
    setLanguege();
    setLayout(layout);
}

void Verification::setLanguege()
{

    verificationtxt->setText(tr("Verification code sent to your email: *****@gmail.com"));
    code->setPlaceholderText(tr("Enter the 6-digit code"));
    chance->setText(tr("You have 3 chances"));
    Next->setText(tr("Verify"));
    Back->setText(tr("Back"));
}

//void Verification::setTextForElements(const QString &verificationMessage, const QString &placeholder, const QString &chanceMessage,
//                                      const QString &VerMss, const QString &BackMs)
//{
//    verificationtxt->setText(verificationMessage);
//    code->setPlaceholderText(placeholder);
//    chance->setText(chanceMessage);
//    Next->setText(VerMss);
//    Back->setText(BackMs);
//}


void Verification::onPrevClicked()
{
    qDebug() << "Previous button clicked!";
    chanceCnt = 3;
    chance->setText("You have 3 chances");
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
    static int chance = 3;
    if (chance <= 0) {
        emit prevClicked();
    } else {
        if (jsonObject.contains("message") && jsonResponse["message"].toString() == "Verification successful") {
            qDebug() << "Verification successful!";
            emit nextClicked();
        } else if (jsonObject.contains("message") && jsonResponse["message"].toString() == "Invalid verification code") {
            qDebug() << "Invalid verification code";
            --chance;
        } else {
            --chance;
            qDebug() << "NEMA";
        }
    }
}


Verification::~Verification() {
    delete verificationtxt;
    delete code;
    delete chance;
    delete Back;
    delete Next;
}
