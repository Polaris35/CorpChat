#include "../include/connection.h"
#include <QDebug>
namespace net
{

Connection::Connection(QObject *parent)
    : QObject(parent)
    , m_socket{nullptr}
    , m_blockSize{0}
    //, m_stream{m_socket}
{
    m_serializer = new PackageJsonSerializer();
    //m_stream.setVersion(QDataStream::Qt_5_15);

    connect(this, &Connection::newPackage, this, &Connection::messageReceived);
}

quint64 Connection::port() const
{
    return m_socket->peerPort();
}


QHostAddress Connection::adress() const
{
    return m_socket->peerAddress();
}


IPackageSerializer *Connection::serializer() const
{
    return m_serializer;
}

void Connection::setSerializer(IPackageSerializer *serializer)
{
    m_serializer = serializer;
}

void Connection::setSocketDescriptor(qintptr descriptor)
{
    m_socket->setSocketDescriptor(descriptor);
}

void Connection::connectToHost(QString host, quint64 port)
{
    if(m_socket->isOpen()) disconnect();

    m_socket->connectToHost(host,port);
    qDebug() << "Connection should be established";

}

void Connection::disconnect()
{
    m_socket->disconnectFromHost();
    emit disconnected();
}

void Connection::messageReceived(const Package &package)
{
    Q_UNUSED(package)
}

void Connection::connected()
{
    emit readyForUse();
    qDebug() << Q_FUNC_INFO << "Connected!";
}

void Connection::error(QAbstractSocket::SocketError socketError)
{
    qCritical() << "Error:" << socketError << " " << m_socket->errorString();
}

void Connection::stateChanged(QAbstractSocket::SocketState socketState)
{
    m_state = socketState;
}

void Connection::sendPackage(Package package)
{
    if(!m_socket->isOpen())
    {
        qDebug() << Q_FUNC_INFO << "Not connected";
        return;
    }
    QByteArray block;
    QByteArray rawData = m_serializer->toBytes(package);
    QDataStream out(&block, QIODevice::WriteOnly);

    out << quint64(rawData.size()) << rawData;
    qDebug() << "Data size should be: " << rawData.size();
    m_socket->write(block);
}

void Connection::readyRead()
{
    qDebug() << Q_FUNC_INFO << m_socket->bytesAvailable();
    QDataStream in(m_socket);

    if(m_blockSize == 0)
    {
        if(m_socket->bytesAvailable() < int(sizeof(quint16)))
        {
            qDebug() << "Can't read size";
            return;
        }

        in >> m_blockSize;
//        qDebug() << "Block size = " << m_blockSize;
    }

    if(m_socket->bytesAvailable() < m_blockSize) {
        qDebug() << "Not enough data. Waiting for next part";
        return;
    } else {
//        qDebug() << m_socket->bytesAvailable() << "should be enough. Reading...";
        m_blockSize = 0;
    }
    QByteArray rawData;

    in >> rawData;
    if(rawData.size() == 0) {
        qDebug() << "Data size == 0";
    }
    Package message;
    bool ok;

    message = m_serializer->fromBytes(rawData, &ok);

    if(!ok)
    {
        qDebug() << Q_FUNC_INFO << "Deserializing went wrong!";
        return;
    }

    emit newPackage(message);
}

QTcpSocket *Connection::socket() const
{
    return m_socket;
}

void Connection::setSocket(QTcpSocket *socket)
{
    m_socket = socket;

    connect(m_socket,&QTcpSocket::connected,this,&Connection::connected);
    connect(m_socket,&QTcpSocket::disconnected,this,&Connection::disconnected);

    connect(m_socket,&QTcpSocket::stateChanged,this,&Connection::stateChanged);
    connect(m_socket,&QTcpSocket::readyRead,this,&Connection::readyRead);
    connect(m_socket,qOverload<QAbstractSocket::SocketError>(&QAbstractSocket::errorOccurred),this,&Connection::error);
}

void Connection::createSocket()
{
    QTcpSocket *sock = new QTcpSocket(this);
    setSocket(sock);
}

}
