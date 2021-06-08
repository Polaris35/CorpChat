#include "../include/client.h"
#include <QImage>
#include <QBuffer>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTime>
#include<QFileInfo>

#include <limits>

#include "../include/globals.h"

#include "../include/contactslist.h"
#include "../include/contactsmodel.h"

#include "../include/messageslist.h"
#include "../include/messagesmodel.h"

#include "../include/userdata.h"


Client::Client(QObject *parent) : QObject(parent),
    m_user{nullptr},
    m_contactsModel{nullptr},
    m_messagesModel{nullptr}

{
    connect(&m_connection, &net::Connection::newPackage, this, &Client::packageRecieved);

}

void Client::registerNewUser(QString username, QString email, QString password, QString imgUrl)
{
    net::Package package;
    QString fixedUrl = imgUrl.remove(0,8); // Удаляем "file:///" с начала пути
    //email$$$username
    package.setSender(email+ net::Package::delimiter() + username);
    package.setType(net::Package::DataType::REGISTRATION_REQUEST);
    package.setDestinations({password});
    QString avatarBase64 = ImageSerializer::toBase64(fixedUrl);

    package.setData(avatarBase64);

    qDebug() << Q_FUNC_INFO << username << email << password << fixedUrl;
    qDebug() << Q_FUNC_INFO << package.data().toByteArray();
    m_connection.sendPackage(package);
}

void Client::authorize(QString email, QString password)
{
    net::Package package;

    package.setType(net::Package::DataType::AUTH_REQUEST);
    package.setSender(email);
    package.setDestinations({});

    package.setData(password);

    m_connection.sendPackage(package);
}

void Client::authorize(QString username, QString email, QByteArray base64)
{

    qDebug() << Q_FUNC_INFO << " autorize user with: email = " << email << " username = " << username;
    QString format = QString(base64.toStdString().c_str()).split("%%%").at(0);
    QString path = QDir::currentPath()
            //+ QDir::separator()
            + QLatin1String("/downloads/")
            //+ QDir::separator()
            + email + "_avatar." + format;

    ImageSerializer::fromBase64(base64,path);
    m_user->setUsername(username);
    m_user->setEmail(email);
    m_user->setImageUrl(path);
}

void Client::sendMessage(QString text)
{
    net::Package package;
    QString delim = net::Package::delimiter();
    QString currentTime = QDateTime::currentDateTime().toString();
    package.setType(net::Package::DataType::TEXT_MESSAGE);
    package.setSender(m_user->email());
    package.setDestinations({m_contactsModel->currentDialog()});
    package.setData(currentTime + delim + text);

    qDebug() << Q_FUNC_INFO << "sending " << text << " to " << m_contactsModel->currentDialog() << " from " << package.sender();

    m_connection.sendPackage(package);
    //m_messagesModel->append(Message{m_user->username(), text, currentTime});
    newMessage(package.sender(),currentTime,text);
}

void Client::sendImage(QString url)
{
    net::Package package;
    QString delim = net::Package::delimiter();
    QString currentTime = QDateTime::currentDateTime().toString();

    QString fixedUrl = url.remove(0,8); //remove "file:///"

    QFileInfo img(fixedUrl);

    QString FileName = img.fileName();

    package.setType(net::Package::DataType::IMAGE);
    package.setSender(m_user->email());
    package.setDestinations({m_contactsModel->currentDialog()});

    qDebug() << Q_FUNC_INFO << "sending Image" << " to " << m_contactsModel->currentDialog() << " from " << package.sender();

    QByteArray imageBase64 = ImageSerializer::toBase64(url);
    qDebug()<< Q_FUNC_INFO << "image size " << imageBase64.size();    

    package.setData(FileName + delim + currentTime /*+ delim + QFile(url).fileName()*/ + delim + imageBase64);

    m_connection.sendPackage(package);

    newImage(package.sender(),FileName, currentTime, imageBase64);
}

void Client::sendDocument(QString url)
{
    net::Package package;
    QString delim = net::Package::delimiter();
    QString currentTime = QDateTime::currentDateTime().toString();

    QString fixedUrl = url.remove(0,8); //remove "file:///"

    QFileInfo doc(fixedUrl);

    QString FileName = doc.fileName().replace(" ","_");

    package.setType(net::Package::DataType::DOCUMENT);
    package.setSender(m_user->email());
    package.setDestinations({m_contactsModel->currentDialog()});

    qDebug() << Q_FUNC_INFO << "sending Document" << " to " << m_contactsModel->currentDialog() << " from " << package.sender();

    QByteArray rawDoc = DocumentSerializer::toByte(fixedUrl);
    qDebug()<< Q_FUNC_INFO << "document size " << rawDoc.size();

    package.setData(FileName + delim + currentTime  + delim + rawDoc);

    m_connection.sendPackage(package);

    newDocument(package.sender(),FileName, currentTime, rawDoc);

}

void Client::getContactsList()
{
    net::Package package;
    package.setSender(m_user->email());
    package.setDestinations({""});
    package.setType(net::Package::DataType::CONTACTS_LIST);
    package.setData({""}); //Don't need any data here. Placeholder for furher rework

    m_connection.sendPackage(package);
}

void Client::getMessageHistory()
{
    static QString lastSelect;
    QString user = m_contactsModel->currentDialog();
    if(lastSelect == user) {
        return;
    } else {
        lastSelect = user;
    }
    qDebug() << "Getting meesage jistory with user = " << user;
    net::Package package;
    package.setSender(m_user->email());
    package.setDestinations({user});
    package.setType(net::Package::DataType::MESSAGE_HISTORY);
    package.setData("");

    m_connection.sendPackage(package);
}

void Client::loadMessageHistory(const QStringList &json)
{
    if(!m_messagesModel)
    {
        return;
    }
    if(json.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Array of message history is empty!";
    }
    m_messagesModel->reset();
    qDebug() << json.size();
    foreach(QString val, json)
    {
        QStringList list = val.split("###");
        qDebug() << "list size :" << list.size();
        if(list.at(0) == "text"){
            newMessage(list.at(1));
            //qDebug() << list.at(1);
        }
        else if(list.at(0) == "image"){
            newImage(list.at(1));
            qDebug() << "new Image in message history!";
        }
        else if(list.at(0) == "document"){
            newDocument(list.at(1));
            qDebug() << "new Document in message history!";
        }
    }
}

void Client::addContact(const QString &contactData)
{
    //email+nick+avatar+isRead
    Contact item;
    QStringList data = contactData.split(net::Package::delimiter());

    item.email = data.at(0);
    item.nickname = data.at(1);

    QString format = data.at(2).split("%%%").first();

    qDebug() << Q_FUNC_INFO << "add contact with: email = " << item.email << " and nickname = " << item.nickname;

    QString path = QDir::currentPath()            
            + QLatin1String("/downloads/")            
            + item.email + "_avatar." + format;

    item.imageUrl = path;

    QByteArray imgRaw = data.at(2).toLocal8Bit();

    //qDebug() << data;

    ImageSerializer::fromBase64(imgRaw,path);
    contactsModel()->append(item);

    if(data.count() == 5)
        if(data.at(3).toInt() == 1) {
            m_toNotify << data.at(0);
        }
}

void Client::newMessage(QString sender, QString time, QString text)
{
    if(sender == m_contactsModel->currentDialog() || sender == m_user->email())
    {
        Message item;
        item.sender = sender;
        item.data = text;
        item.timeStamp = time;
        item.type = "text";
        m_messagesModel->append(item);
    }
    else
    {
        emit notifyMessage(sender);
    }

}

void Client::newImage(QString raw)
{
    //nick$$$filename$$$time$$$image
    QStringList data = raw.split(net::Package::delimiter());
    QString sender = data.at(0);
    QString filename = data.at(1);
    QString time = data.at(2);
    QByteArray img = data.at(3).toUtf8();

    newImage(sender,filename,time,img);
}


void Client::newImage(QString sender, QString filename, QString time, QByteArray base64)
{    
    if(sender == m_contactsModel->currentDialog() || sender == m_user->email())
    {
        QString path = QDir::currentPath()
                + QLatin1String("/downloads/")
                + filename;
        qDebug() <<Q_FUNC_INFO << path;
        ImageSerializer::fromBase64(base64,path);
        Message item;
        item.sender = sender;
        item.data = path;
        item.timeStamp = time;
        item.type = "image";
        m_messagesModel->append(item);
    }
    else
    {
        emit notifyMessage(sender);
    }
}

void Client::newMessage(QString raw)
{
    //"nickname$$$time$$$text$$$
    QStringList data = raw.split(net::Package::delimiter());
    QString sender = data.at(0);
    QString time = data.at(1);
    QString text = data.at(2);

    newMessage(sender,time,text);
}

void Client::newDocument(QString sender, QString filename, QString time, QByteArray base64)
{
    if(sender == m_contactsModel->currentDialog() || sender == m_user->email())
    {
        QString path = QDir::currentPath()
                + QLatin1String("/downloads/")
                + filename;
        qDebug() <<Q_FUNC_INFO << path;
        DocumentSerializer::fromByte(base64,path);
        Message item;
        item.sender = sender;
        item.data = path;
        item.timeStamp = time;
        item.type = "document";
        m_messagesModel->append(item);
    }
    else
    {
        emit notifyMessage(sender);
    }
}

void Client::newDocument(QString raw)
{    
    //nick$$$filename$$$time$$$doc
    QStringList data = raw.split(net::Package::delimiter());
    QString sender = data.at(0);
    QString filename = data.at(1);
    QString time = data.at(2);
    QByteArray doc = data.at(3).toUtf8();
    qDebug() << "We have a document " << filename;
    newDocument(sender,filename,time,doc);
}



void Client::requestContact(QString email)
{
    qDebug() << Q_FUNC_INFO << "GIVE ME " << email;
    if(m_contactsModel->list()->exists(email)) {
        return;
    }
    net::Package item;
    item.setSender(m_user->email());
    item.setType(net::Package::USER_DATA);
    item.setDestinations({email});
    item.setData("");

    m_connection.sendPackage(item);
}

void Client::createConversation(QString title, QString path, QStringList users)
{
    QString fixedPath = path.remove(0,8); //remove "file:///"

    net::Package item;
    item.setSender(title + net::Package::delimiter() + m_user->email());
    item.setType(net::Package::CreateConversation);
    item.setDestinations(users);
    item.setData(ImageSerializer::toBase64(fixedPath));

    m_connection.sendPackage(item);
}

void Client::qmlNotifyUnreadMessage(QString sender)
{
    if(m_toNotify.contains(sender))
        emit notifyMessage(sender);
}



UserData *Client::getUser() const
{
    return m_user;
}

void Client::setUser(UserData *user)
{
    m_user = user;
}

QString Client::username() const
{
    return m_user->username();
}

void Client::start()
{
    m_connection.createSocket();
    m_connection.connectToHost("26.108.219.52", 52484);
    qDebug() << "Client started";
}

void Client::start(QString hostIp, quint64 port)
{
    m_connection.createSocket();
    qDebug() << "Connecting to " << hostIp << port;
    m_connection.connectToHost(hostIp, port);
    qDebug() << "Client started on " << hostIp << port;
}

MessagesModel *Client::messagesModel() const
{
    return m_messagesModel;
}

void Client::setMessagesModel(MessagesModel *messagesModel)
{
    if(m_messagesModel)
        m_messagesModel->disconnect(this);
    m_messagesModel = messagesModel;
}

ContactsModel *Client::contactsModel() const
{
    return m_contactsModel;
}

void Client::setContactsModel(ContactsModel *contactsModel)
{
    if(m_contactsModel)
        m_contactsModel->disconnect(this);

    m_contactsModel = contactsModel;
    connect(m_contactsModel, &ContactsModel::selectedChanged, this, &Client::getMessageHistory);
}


void Client::packageRecieved(net::Package package)
{
    QByteArray data = package.data().toByteArray();
    qDebug() << "Recieved package of type" << package.type();
    switch(package.type())
    {
    case net::Package::CONTACTS_LIST:
        //qDebug() << package.data();

        loadContactsList(package.data().toStringList());
        break;
    case net::Package::REGISTRATION_REQUEST:
        if(data.startsWith("S"))
            emit registerSuccess();
        else
            emit registerFailure();
        break;

    case net::Package::AUTH_REQUEST: //Ответ на авторизацию Данные пустые = фейл
        if(!data.isEmpty() && !package.destinations().empty() && package.sender() != "")
        {
            authorize(package.sender(),package.destinations().first(), data);
            emit authSuccsess();
            getContactsList();
        }
        else
            emit authFailure();
        break;
    case net::Package::MESSAGE_HISTORY: //Переписка с контактом. Загружем при переключении на другой диалог
        loadMessageHistory(package.data().toStringList());
        break;
    case net::Package::USER_DATA: //Добавляем 1 контакт
        addContact(data);
        break;
    case net::Package::TEXT_MESSAGE: //Текстовое сообщение
        newMessage(package.sender() + net::Package::delimiter() + data); //никнейм$$$время$$$текст
        break;
    case net::Package::IMAGE:
        newImage(package.sender() + net::Package::delimiter() + data); //nick$$$filename$$$time$$$image
        break;
    case net::Package::DOCUMENT:
        newDocument(package.sender() + net::Package::delimiter() + data); //nick$$$filename$$$time$$$doc
        break;
    case net::Package::GROUP_MESSAGE_HISTORY:
        loadConversationList(package.data().toStringList());
        break;
    case net::Package::AddtoConversation:
        break;
    case net::Package::CreateConversation:
        break;
    }
}

void Client::loadContactsList(const QStringList &json)
{
    if(json.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "Array is empty";
        return;
    }
    foreach(QString contact, json)
    {
        qDebug() << Q_FUNC_INFO << "Invoking addContact";
        addContact(contact);
    }
}

void Client::loadConversationList(const QStringList &json)
{
    //ID$$$title$$$AvatarRaw$$$creatorEmail$$$size$$$userdata
//    foreach(QString item, json)
//    {
//        QStringList data = item.split(net::Package::delimiter());
//        int id = data.at(0).toInt();
//        QString title = data.at(1);
//        QString avatar = data.at(2);
//        QString creator_email = data.at(3);
//        int size = data.at(4).toInt();
//        for(int i = 5; i < size/3;i+=3) //email$$$nickname$$$avataUrl
//        {
//            QString email = data.at(i);
//            QString nickname = data.at(i+1);
//            QString userAvatar = data.at(i+2);
//        }

//    }

}
