#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QString& username, QObject *parent = nullptr);
    void sendMessage(const QString &recipient, const QString &message);
    void closeConnection();

signals:
    void messageReceived(const QString &sender, const QString &message, const QString &timestamp);

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
    void onDisconnected();

private:
    QWebSocket m_webSocket;
    QString m_username;
};

#endif // WEBSOCKETCLIENT_H
