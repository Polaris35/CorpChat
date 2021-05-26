#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QMetaEnum>
#include <QDataStream>

#include "packagejsonserializer.h"
#include "CorpChatNetworkLib_global.h"
#include "package.h"
namespace net
{

class CORPCHATNETWORKLIB_EXPORT Connection : public QObject
{
    Q_OBJECT

public:
    explicit Connection(QObject *parent = nullptr);

    quint64 port() const;
    QHostAddress adress() const;

    IPackageSerializer *serializer() const;
    void setSerializer(IPackageSerializer *serializer);

    void setSocketDescriptor(qintptr descriptor);

    void sendPackage(Package package);
    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *socket);

    void createSocket();
public slots:
    void connectToHost(QString host, quint64 port);
    void disconnect();

    void messageReceived(const Package &package);
signals:
    void readyForUse();
    void newPackage(const Package &package);
    void disconnected();

private slots:
    void connected();
    void error(QAbstractSocket::SocketError socketError);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void readyRead();
private:
    //QDataStream m_stream;
    QTcpSocket *m_socket;
    IPackageSerializer *m_serializer;
    QAbstractSocket::SocketState m_state;
    quint64 m_blockSize;

}; //class Connection

} //namespace net
#endif // CONNECTION_H
