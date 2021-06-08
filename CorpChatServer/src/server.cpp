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
    case net::Package::CONTACTS_LIST: //Спислк контактов. Получаем сразу после входа в аккаунт
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
    case net::Package::USER_DATA: //Добавляем 1 контакт  // создаем переписку между двумя контактами
        newConversation(package.sender(),package.destinations().first());
        break;
    case net::Package::TEXT_MESSAGE: //Текстовое сообщение
        sendMessage(package);
        break;
    case net::Package::IMAGE: // Send images to user
        sendImage(package);
        break;
    case net::Package::DOCUMENT: // Send docs to user        
        sendDocument(package);
        break;
    case net::Package::CreateConversation:
        CreateConversation(package);
        break;
    case net::Package::AddtoConversation: // добавляет пользователей(package.destinations()) в груповой чат
        addUsersToConversation(package.data().toInt(), package.destinations());
        break;
    case net::Package::GROUP_MESSAGE_HISTORY: // возвращает историю сообщений для групового чата
        sendMessageHistory(package.sender(), package.data().toInt());
        break;
    case net::Package::CONVERSATION_REQUEST: // отправляем все груповые чаты в которых есть пользователь
        break;
    }
}

void Server::registerUser(net::Package package, net::Connection *connection)
{
    qInfo() << Q_FUNC_INFO;
    //email$$$username
    QStringList raw = package.sender().split(net::Package::delimiter());
    QString email = raw.first();
    QString username = raw.last();
    QString password = package.destinations().toVector().first();

    QString format = package.data().toString().split("%%%").at(0);

    qDebug() << "image format is " << format;


    qDebug() << "PW: " << password;

    QString path = QDir::currentPath()            
            + QLatin1String("/downloads/")            
            + email + "_avatar." + format;

    ImageSerializer::fromBase64(package.data().toByteArray(),path);

    //Отправляем клиенту результат регистрации
    net::Package responce;
    responce.setSender("");
    responce.setDestinations({email});
    responce.setType(net::Package::DataType::REGISTRATION_REQUEST);
    if(m_database->registerUser(username, email, password, path))
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
    QString email = package.sender();
    QString username = m_database->userNickname(email);
    QString password = package.data().toString();
    if(m_database->authorizeUser(email,password))
    {
        m_clients[email] = connection;
        net::Package item;

        item.setSender(username);
        item.setDestinations({email});
        item.setType(net::Package::DataType::AUTH_REQUEST);
        QString url = m_database->userImage(email);
        QString avatarBase64 = ImageSerializer::toBase64(url);
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

void Server::sendMessageHistory(const QString& to, const int &conversation_id)
{
    QStringList messageHistory = m_database->messageHistory(conversation_id);

    net::Package item;
    item.setSender("");
    item.setType(net::Package::GROUP_MESSAGE_HISTORY);
    item.setDestinations({to});
    item.setData(messageHistory);

    m_clients.value(to)->sendPackage(item);
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

void Server::sendContactsList(QString email_user)
{
    //nickname, url
    QHash<QString, QString> contacts = m_database->contactsList(email_user);
    //email+url+url
    net::Package item;
    item.setSender("");
    item.setType(net::Package::DataType::CONTACTS_LIST);
    item.setDestinations({email_user});
    QStringList formattedContacts;
    QHashIterator<QString, QString> i(contacts);
    while(i.hasNext()) {
        i.next();
        QString email = i.key().split(net::Package::delimiter()).first();
        int hasUnread = i.key().split(net::Package::delimiter()).first().toInt();
        QByteArray base64 = ImageSerializer::toBase64(i.value());
        qDebug()<< Q_FUNC_INFO << "Nickname is: " <<  m_database->userNickname
                   (email.split(net::Package::delimiter()).first());


        formattedContacts.append(email + net::Package::delimiter()//email+nick+avatar+isRead
                                 + m_database->userNickname(email.split(net::Package::delimiter()).first())
                                 + net::Package::delimiter() + base64 + net::Package::delimiter() +
                                 QString::number(hasUnread));

    }
    qDebug() << Q_FUNC_INFO << formattedContacts.size();
    item.setData(formattedContacts);

    m_clients.value(email_user)->sendPackage(item);
}

void Server::sendConversationList(QString user)
{
    QList<int> ids = m_database->conversationList(user);
    QStringList data;
    foreach(int id, ids)
    {
        QStringList temp(m_database->getConversationData(id).split(net::Package::delimiter()));
        temp[0] = QString::number(id) +
                net::Package::delimiter() + temp.at(0);
        temp[1] = ImageSerializer::toBase64(temp.at(1));
        QString rezult = "";
        foreach(const QString& str, temp)
            rezult += str;
        data.append(temp);//ID$$$title$$$AvatarPath$$$creatorEmail$$$size$$$userdata
    }
    qDebug() << Q_FUNC_INFO << "conversation list have size: " << data.size();

    net::Package item;
    item.setSender("");
    item.setType(net::Package::GROUP_MESSAGE_HISTORY);
    item.setDestinations({user});
    item.setData(data);

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

void Server::sendImage(const net::Package &package)
{
    QString sender = package.sender();
    QStringList destinations = package.destinations();
    QStringList data = package.data().toString().split(net::Package::delimiter());
    QString filename = data.at(0);
    qDebug() << Q_FUNC_INFO << "filename is:" << filename;
    QString dateTime = data.at(1);
    QByteArray image = data.at(2).toUtf8();
    m_database->newImage(sender,destinations,filename,image,dateTime);

    foreach(QString dest, destinations) {
        if(m_clients.contains(dest)) {
            m_clients.value(dest)->sendPackage(package);
        }
    }
}

void Server::sendDocument(const net::Package &package)
{
    QString sender = package.sender();
    QStringList destinations = package.destinations();
    QStringList data = package.data().toString().split(net::Package::delimiter());
    QString filename = data.at(0);
    qDebug() << Q_FUNC_INFO << "filename is:" << filename;
    QString dateTime = data.at(1);
    QByteArray image = data.at(2).toUtf8();
    m_database->newDocument(sender,destinations,filename,image,dateTime);

    foreach(QString dest, destinations) {
        if(m_clients.contains(dest)) {
            m_clients.value(dest)->sendPackage(package);
        }
    }
}

void Server::CreateConversation(const net::Package &package)
{
     // title+sender
    QStringList raw = package.sender().split(net::Package::delimiter());
    QString title = raw.first();
    QString creator = raw.last();
    QStringList users = package.destinations();
    QString format = package.data().toString().split("%%%").first();

    QString path = QDir::currentPath() + QLatin1String("/downloads/") + "Conversation_" +
            QUuid::createUuid().toString().remove("{").remove("}") + "_." + format;

    ImageSerializer::fromBase64(package.data().toByteArray(), path);

    if(m_database->createConversation(title, creator, users, path))
    {
        qDebug() << Q_FUNC_INFO << "conversation created succesfuly!";
    }
}



void Server::newConversation(const QString &user1, const QString &user2)
{
    if(m_database->newConversation(user1, user2))
    {
        sendContact(user1,user2);
        sendContact(user2,user1);
    }

}

void Server::addUsersToConversation(const int& conversation_id,const QStringList& users)
{
    net::Package item;

    //id$$title$$$AvatarPath$$$creatorEmail
    QStringList conv_info = m_database->getConversationData(conversation_id).
            split(net::Package::delimiter());

    item.setSender(conv_info.at(1) + conv_info.at(3));
    item.setType(net::Package::AddtoConversation);
    item.setDestinations(users);
    QByteArray rawimg = ImageSerializer::toBase64(conv_info.at(2));
    item.setData(rawimg);


    foreach(QString user, users)
    {

        int user_id = m_database->userID(user);
        if(user_id == -1)
            continue;
        m_database->addUserToConversation(conversation_id, user_id);
        if(!m_clients.contains(user)) {
            //qWarning() << Q_FUNC_INFO << "User " << to << "not online";
            return;
        }
    }
}

void Server::sendContact(const QString &to, const QString &other)
{
    qDebug() << Q_FUNC_INFO << "sending contact to: " << to;
    if(!m_clients.contains(to)) {
        //qWarning() << Q_FUNC_INFO << "User " << to << "not online";
        return;
    }
    net::Package item;
    item.setSender("");
    item.setType(net::Package::USER_DATA);
    item.setDestinations({to});
    QByteArray imageBase64 = ImageSerializer::toBase64(m_database->userImage(other));
    QString userData = other + net::Package::delimiter() +
            m_database->userNickname(other) + net::Package::delimiter() + imageBase64;

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
