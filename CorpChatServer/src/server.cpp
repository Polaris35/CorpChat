#include "../include/server.h"
#include "../include/dbfacade.h"

#include <QDir>

Server::Server(QObject *parent)
    : QObject(parent)
{
    qInfo() << connect(&m_server, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::newConnection()
{
    qInfo() << "Connecting...";
    QTcpSocket *socket = m_server.nextPendingConnection();

    net::Connection *connection = new net::Connection(this);
    connection->setSocket(socket);

    qInfo() << "Connected" << socket->socketDescriptor();
    connect(connection, &net::Connection::newPackage, this, &Server::newPackage);
    connect(connection, &net::Connection::disconnected, this, &Server::disconnected);
}

void Server::disconnected()
{
    net::Connection *connection = qobject_cast<net::Connection *>(sender());
    QString toDelete = m_clients.key(connection);
    connection->deleteLater();
    m_clients.remove(toDelete);
}

void Server::newPackage(const net::Package &package)
{
    qInfo() << Q_FUNC_INFO << "Package recieved successfully!";
    net::Connection *connection = qobject_cast<net::Connection *>(sender());

    switch(package.type())
    {
    case net::Package::CONTACTS_LIST: //Спислк контактов. Получаем сразе после входа в аккаунт
        sendContactsList(package.sender());
        break;
    case net::Package::REGISTRATION_REQUEST:
        registerUser(package,connection);
        break;
    case net::Package::AUTH_REQUEST:
        authorize(package, connection);
        break;
    case net::Package::MESSAGE_HISTORY: //Переписка с контактом. Загружем при переключении на другой диалог
        sendMessageHistory(package.sender(), package.destinations());
        break;
    case net::Package::USER_DATA: //Добавляем 1 контакт
        newConversation(package.sender(),package.destinations().first());
        break;
    case net::Package::TEXT_MESSAGE: //Текстовое сообщение
        sendMessage(package);
        break;
    case net::Package::IMAGE: // Send to user images
        sendMessage(package);
        break;
    case net::Package::DOCUMENT:

        break;
    }
}

void Server::registerUser(net::Package package, net::Connection *connection)
{
    qInfo() << Q_FUNC_INFO;
    //QList<QByteArray> data = package.data().toByteArray().split('$');
//    if(data.size() < 2) {
//        qDebug() << "Can't split data";
//        return;
//    }
    QString username = package.sender();
    QString password = package.destinations().toVector().first();
    qDebug() << "PW: " << password;
//    qDebug() << "DATA: " << data.last();
    QString path = QDir::currentPath()
            //+ QDir::separator()
            + QLatin1String("/images/")
            //+ QDir::separator()
            + username + "_avatar.png";

    ImageSerializer::fromBase64(package.data().toByteArray(),path);

    //Отправляем клиенту результат регистрации
    net::Package responce;
    responce.setSender("");
    responce.setDestinations({username});
    responce.setType(net::Package::DataType::REGISTRATION_REQUEST);
    if(m_database->registerUser(username, password, path))
    {
        responce.setData(QStringLiteral("S"));
    } else {
        responce.setData(QStringLiteral("F"));
    }
    connection->sendPackage(responce);
}

void Server::authorize(net::Package package, net::Connection *connection)
{
    qInfo() << Q_FUNC_INFO;
    QString username = package.sender();
    QString password = package.data().toString();
    if(m_database->authorizeUser(username,password))
    {
        m_clients[username] = connection;
        net::Package item;
        item.setSender("");
        item.setDestinations({username});
        item.setType(net::Package::DataType::AUTH_REQUEST);
        QString url = m_database->userImage(username);
        QString avatarBase64 = ImageSerializer::toBase64(url);//imgRaw.toBase64();
        item.setData(avatarBase64);

        connection->sendPackage(item);
    } else {
        qDebug() << "User not registered";
        net::Package item;
        item.setSender("");
        item.setDestinations({});
        item.setType(net::Package::DataType::AUTH_REQUEST);
        item.setData(QVariant(""));
        connection->sendPackage(item);
    }
}

void Server::sendMessageHistory(const QString &to, const QStringList &conversants)
{
    QStringList messageHistory = m_database->messageHistory(to, conversants.first());

    net::Package item;
    item.setSender(conversants.first());
    item.setType(net::Package::MESSAGE_HISTORY);
    item.setDestinations({to});
    item.setData(messageHistory);

    m_clients.value(to)->sendPackage(item);
}

void Server::sendContactsList(QString user)
{
    //    nickname, url
    QHash<QString, QString> contacts = m_database->contactsList(user);

    net::Package item;
    item.setSender("");
    item.setType(net::Package::DataType::CONTACTS_LIST);
    item.setDestinations({user});
    QStringList formattedContacts;
    QHashIterator<QString, QString> i(contacts);
    while(i.hasNext()) {
        i.next();
        QString nickname = i.key();
        QByteArray base64 = ImageSerializer::toBase64(i.value());
        formattedContacts.append(nickname + net::Package::delimiter() + base64);
    }
    qDebug() << Q_FUNC_INFO << formattedContacts.size();
    item.setData(formattedContacts);

    m_clients.value(user)->sendPackage(item);
}

void Server::sendMessage(const net::Package &package)
{
    QString sender = package.sender();
    QStringList destinations = package.destinations();
    QStringList data = package.data().toString().split(net::Package::delimiter());
    QString dateTime = data.first();
    QString text = data.last();
    m_database->newMessage(sender,destinations,text,dateTime);

    foreach(QString dest, destinations) {
        if(m_clients.contains(dest)) {
            m_clients.value(dest)->sendPackage(package);
        }
    }
}

void Server::newConversation(const QString &user1, const QString &user2)
{
   if(m_database->newConversation(user1, user2))
   {
        sendContact(user1,user2);
        sendContact(user2,user1);
   }
//   else
//   {
//       net::Package item;
//       item.setSender("");
//       item.setDestinations({user1});
//   }
}

void Server::sendContact(const QString &to, const QString &other)
{
    if(!m_clients.contains(to)) {
        //qWarning() << Q_FUNC_INFO << "User " << to << "not online";
        return;
    }
    net::Package item;
    item.setSender("");
    item.setType(net::Package::USER_DATA);
    item.setDestinations({to});
    QByteArray imageBase64 = ImageSerializer::toBase64(m_database->userImage(other));
    QString userData = other + net::Package::delimiter() + imageBase64;

    item.setData(userData);

    m_clients.value(to)->sendPackage(item);
}

void Server::setDatabase(DBFacade *database)
{
    m_database = database;
}

void Server::start()
{
    if(m_server.listen(QHostAddress("26.108.219.52"), 52484))
        qInfo() << "Listening on " << m_server.serverAddress().toString() << ":" << m_server.serverPort();
    else
        qInfo() << "Server not started!";
}
