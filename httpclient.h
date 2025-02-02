#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QUrl>
#include <QJsonObject>

class QNetworkAccessManager;
class QNetworkReply;

class HttpClient : public QObject {
    Q_OBJECT
public:
    explicit HttpClient(QObject* parent = nullptr);

    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;


    void getRequest(const QUrl& url);
    void postRequest(const QUrl& url, const QJsonObject& data);
    void putRequest(const QUrl& url, const QJsonObject& data);
    void deleteRequest(const QUrl& url);
    void uploadFile(const QUrl& url, const QString& filePath);

private slots:
    void onReply();

private:
    QNetworkAccessManager* manager;
};

#endif // HTTPCLIENT_H
