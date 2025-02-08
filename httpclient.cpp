#include "httpclient.h"

#include <QDebug>
#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void HttpClient::getRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onReply);
}

void HttpClient::postRequest(const QUrl &url, const QJsonObject &data)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument jsonDoc(data);
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onReply);
}

void HttpClient::putRequest(const QUrl &url, const QJsonObject &data)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument jsonDoc(data);
    QNetworkReply *reply = manager->put(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onReply);
}

void HttpClient::deleteRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onReply);
}

void HttpClient::uploadFile(const QUrl &url, const QString &filePath)
{
    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file";
        file->deleteLater();
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" + file->fileName() + "\""));
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);

    QNetworkRequest request(url);
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onReply);
}

void HttpClient::onReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        QByteArray responseData = reply->readAll();
        qDebug() << "Response:" << responseData;

        emit responseReceived(responseData);
        reply->deleteLater();
    }
}
