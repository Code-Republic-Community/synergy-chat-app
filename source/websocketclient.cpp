#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include "websocketclient.h"
#include "globals.h"

WebSocketClient::WebSocketClient(const QString& username, QObject *parent)
    : QObject(parent), m_username(username)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);

    QString wsUrl = Globals::getInstance().getWEB_SOCKET_LINK() + m_username;
    m_webSocket.open(QUrl(wsUrl));
}

void WebSocketClient::onConnected()
{
    qDebug() << "Connected to WebSocket server!";
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    QString sender = obj["sender"].toString();
    QString msg = obj["message"].toString();
    QString timestamp = obj["timestamp"].toString();

    emit messageReceived(sender, msg, timestamp);
}

void WebSocketClient::sendMessage(const QString &recipient, const QString &message)
{
    if (m_webSocket.isValid()) {
        QJsonObject obj;
        obj["recipient"] = recipient;
        obj["message"] = message;

        QJsonDocument doc(obj);
        m_webSocket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void WebSocketClient::onDisconnected()
{
    qDebug() << "Disconnected from WebSocket server!";
}

void WebSocketClient::closeConnection()
{
    m_webSocket.close();
}
