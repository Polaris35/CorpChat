#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QVariant>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

#include "include/CorpChatNetworkLib_global.h"
#include "include/connection.h"
#include "include/package.h"
#include "include/imageserializer.h"

class DBFacade;

class Server : public QObject
{
    Q_OBJECT


public:
    explicit Server(QObject *parent = nullptr);

    void setDatabase(DBFacade *database);
    void start();
private slots:
    void newConnection();
    void disconnected();
    void newPackage(const net::Package &package);

    void registerUser(net::Package package, net::Connection *connection);
    void authorize(net::Package package, net::Connection *connection);
    void sendMessageHistory(const QString &to, const QStringList &conversants);
    void sendContactsList(QString user);
    void sendMessage(const net::Package &package);
    void sendImage(const net::Package& package);
    void newConversation(const QString &user1, const QString &user2);

private:
    void sendContact(const QString &to, const QString &other);

private:
    QTcpServer m_server;
    QHash<QString, net::Connection*> m_clients;
    DBFacade *m_database;
};

#endif // SERVER_H
