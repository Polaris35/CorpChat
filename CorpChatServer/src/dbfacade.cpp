#include "../include/dbfacade.h"
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QUuid>
#include<QFile>
#include<QDir>
#include<QImage>
#include "include/package.h"
#include "include/imageserializer.h"
#include "include/DocumentSerializer.h"

DBFacade::DBFacade(QObject *parent) : QObject(parent)
{

}

bool DBFacade::registerUser(QString username, QString email, QString password, QString imageUrl)
{
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO attachments (id_message, url)"
                  "VALUES (:null, :url);");

    query.bindValue(":null", QVariant());
    query.bindValue(":url", imageUrl);

    if(!query.exec())
    {
        qWarning() << Q_FUNC_INFO << "Inserting image failed" << query.lastError().text();
        return false;
    }
    int id = query.lastInsertId().toInt();
    query.finish();

    query.prepare("INSERT INTO useres (email,password, nickname, id_pictures) "
                  "VALUES (:email, :password,:nickname, :id);");
    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.bindValue(":nickname", username);
    query.bindValue(":id", id);
    if(!query.exec())
    {
        qWarning() << Q_FUNC_INFO << "Inserting user failed" << query.lastError().number();
    }
    query.finish();
    return true;
}

bool DBFacade::authorizeUser(QString email, QString password)
{
    QSqlQuery query(*m_db);
    query.prepare("SELECT COUNT(*) "
                  "FROM useres "
                  "WHERE email = :email AND password = :password");
    query.bindValue(":email",email);
    query.bindValue(":password", password);
    if(query.exec() && query.next())
    {
        bool isRegistered = query.value(0).toInt() != 0;
        return isRegistered;
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Cannot execute query" << query.lastError().number();
        return false;
    }
}

void DBFacade::addMessage(QString sender, QString receiver, QString messageText)
{
    QSqlQuery query(*m_db);

    auto queryFailure = [](){
        qWarning() << Q_FUNC_INFO << "Cannot execute query";
    };

    query.prepare("SELECT id "
                  "FROM conversations "
                  "WHERE (title = CONCAT(:name1, \"$$$\", :name2)) "
                  "OR (title = CONCAT(:name2, \"$$$\", :name1)) ");
    query.bindValue(":name1", sender);
    query.bindValue(":name2", receiver);
    int conversationId;
    if(!query.exec())
    {
        queryFailure();
    }
    if(query.next())
    {
        conversationId = query.value(0).toInt();
    }
    else
    {
        qWarning() << "Conversation between" << sender << "and" << receiver << "doesn't exist";
        return;
    }
    query.finish();
    query.prepare("SELECT id"
                  "FROM useres"
                  "WHERE email = :sender");
    query.bindValue(":sender",sender);
    int senderId;
    if(!query.exec())
    {
        queryFailure();
    }
    if(query.next())
    {
        senderId = query.value(0).toInt();
    } else {
        qWarning() << "User " << sender << "not registered";
        return;
    }
    query.finish();
    query.prepare("INSERT INTO messages(id_sender, id_conversation, created_at"
                  "VALUES(:senderId, :convId, :message, :createdAt)");
    query.bindValue(":senderId", senderId);
    query.bindValue(":convId", conversationId);
    query.bindValue(":message",messageText);
    query.bindValue(":createdAt",QDateTime::currentDateTime());
    if(!query.exec())
    {
        queryFailure();
    }
}

QStringList DBFacade::messageHistory(QString user1, QString user2)
{
    QSqlQuery query(*m_db);
    query.prepare(
                "SELECT u.email, m.created_at, m.message, m.message_type,m.id "
                "FROM messages m "
                "INNER JOIN useres u ON m.id_sender = u.id "
                "WHERE (m.id_conversation IN ( "
                    "SELECT p.id_conversation "
                    "FROM participants p "
                             "INNER JOIN useres u ON p.id_useres = u.id "
                    "WHERE (u.email = :user1 AND p.id_conversation IN ( "
                        "SELECT p2.id_conversation "
                        "FROM participants p2 "
                                 "INNER JOIN useres u2 ON p2.id_useres = u2.id "
                        "WHERE (u2.email = :user2) "
                    ") "
                              ")) "
                          ") "
                "ORDER BY m.created_at");
    query.bindValue(":user1",user1);
    query.bindValue(":user2",user2);
    qDebug() << user1 << user2;
    bool ok = query.exec();
    if(!ok){
        qWarning() << Q_FUNC_INFO << query.lastError().text();
    }
    QStringList messages;
    qDebug() << query.size();
    QSqlQuery secondq;
    int id;
    secondq.prepare("SELECT COUNT(*), url FROM attachments WHERE id_message = :id");

    while(query.next()) {
        if(query.value(3).toString() == "text")
        {
            messages.append( //u.nickname, m.created_at, m.message
                             "text###"                                       +
                             query.value(0).toString()                       + net::Package::delimiter() +
                             query.value(1).toDateTime().toString()          + net::Package::delimiter() +
                             query.value(2).toString());
        }
        else if(query.value(3).toString() == "image")
        {
            id = query.value(4).toInt();
            secondq.bindValue(":id",id);
            if(!secondq.exec()){
                qWarning() << Q_FUNC_INFO
                << "can't recieve an image path!!!"
                << secondq.lastError().text();
            }
            secondq.next();
            QString path;
            if(secondq.value(0).toInt() != 0){
                path = secondq.value(1).toString();
                qDebug() << Q_FUNC_INFO << path;
            }
            else
            {
                qDebug() << Q_FUNC_INFO;
                qFatal("not found image path!!!");

            }
            messages.append(
                        "image###"                                      +
                        query.value(0).toString()                       + net::Package::delimiter() +
                        QFileInfo(path).fileName()                      + net::Package::delimiter() +
                        query.value(1).toDateTime().toString()          + net::Package::delimiter() +
                        ImageSerializer::toBase64(path));
        }
        else if(query.value(3).toString() == "document")
        {
            id = query.value(4).toInt();
            secondq.bindValue(":id",id);
            if(!secondq.exec()){
                qWarning() << Q_FUNC_INFO
                << "can't recieve an document path!!!"
                << secondq.lastError().text();
            }
            secondq.next();
            QString path;
            if(secondq.value(0).toInt() != 0){
                path = secondq.value(1).toString();
                qDebug() << Q_FUNC_INFO << path;
            }
            else
            {
                qDebug() << Q_FUNC_INFO;
                qFatal("not found document path!!!");

            }
            messages.append(
                        "document###"                                   +
                        query.value(0).toString()                       + net::Package::delimiter() +
                        QFileInfo(path).fileName()                      + net::Package::delimiter() +
                        query.value(1).toDateTime().toString()          + net::Package::delimiter() +
                        DocumentSerializer::toByte(path));
        }
    }
    qDebug() << "Message history has size:" << messages.size();

    query.finish();

    query.prepare(
                "UPDATE messages m "
                  "INNER JOIN conversations c ON m.id_conversation = c.id "
                  "INNER JOIN participants p ON c.id = p.id_conversation "
                  "INNER JOIN useres u ON p.id_useres = u.id "
              "SET m.isRead = TRUE "
              "WHERE (m.isRead = FALSE "
                  "AND m.id_conversation IN ( "
                  "SELECT p.id_conversation "
                  "FROM participants p "
                           "INNER JOIN useres u ON p.id_useres = u.id "
                  "WHERE (u.nickname = :user1 AND p.id_conversation IN ( "
                      "SELECT p2.id_conversation "
                      "FROM participants p2 "
                               "INNER JOIN useres u2 ON p2.id_useres = u2.id "
                      "WHERE m.id_sender = (SELECT u3.id FROM useres u3 WHERE u3.nickname = :user2 LIMIT 1) "
                  ") "
                            ")) "
                  "AND u.nickname = :user2 "
                        ")");
    query.bindValue(":user1", user1);
    query.bindValue(":user2", user2);
    if(!query.exec()) {
        qDebug() << Q_FUNC_INFO << "Can't update messages!";
    }
    return messages;
}

QStringList DBFacade::messageHistory(const int &conversation_id)
{
    QSqlQuery query(*m_db);
    query.prepare("SELECT u.email, m.created_at, m.message, m.message_type,m.id "
                  "FROM messages m "
                  "INNER JOIN useres u ON m.id_sender = u.id"
                  "WHERE id_conversation = :id");

    query.bindValue(":id",conversation_id);
    bool ok = query.exec();
    if(!ok){
        qWarning() << Q_FUNC_INFO << query.lastError().text();
    }
    QStringList messages;
    qDebug() << query.size();
    QSqlQuery secondq;
    int id;
    secondq.prepare("SELECT COUNT(*), url FROM attachments WHERE id_message = :id");

    while(query.next()) {
        if(query.value(3).toString() == "text")
        {
            messages.append( //u.nickname, m.created_at, m.message
                             "text###"                                       +
                             query.value(0).toString()                       + net::Package::delimiter() +
                             query.value(1).toDateTime().toString()          + net::Package::delimiter() +
                             query.value(2).toString());
        }
        else if(query.value(3).toString() == "image")
        {
            id = query.value(4).toInt();
            secondq.bindValue(":id",id);
            if(!secondq.exec()){
                qWarning() << Q_FUNC_INFO
                << "can't recieve an image path!!!"
                << secondq.lastError().text();
            }
            secondq.next();
            QString path;
            if(secondq.value(0).toInt() != 0){
                path = secondq.value(1).toString();
                qDebug() << Q_FUNC_INFO << path;
            }
            else
            {
                qDebug() << Q_FUNC_INFO;
                qFatal("not found image path!!!");

            }
            messages.append(
                        "image###"                                      +
                        query.value(0).toString()                       + net::Package::delimiter() +
                        QFileInfo(path).fileName()                      + net::Package::delimiter() +
                        query.value(1).toDateTime().toString()          + net::Package::delimiter() +
                        ImageSerializer::toBase64(path));
        }
        else if(query.value(3).toString() == "document")
        {
            id = query.value(4).toInt();
            secondq.bindValue(":id",id);
            if(!secondq.exec()){
                qWarning() << Q_FUNC_INFO
                << "can't recieve an document path!!!"
                << secondq.lastError().text();
            }
            secondq.next();
            QString path;
            if(secondq.value(0).toInt() != 0){
                path = secondq.value(1).toString();
                qDebug() << Q_FUNC_INFO << path;
            }
            else
            {
                qDebug() << Q_FUNC_INFO;
                qFatal("not found document path!!!");

            }
            messages.append(
                        "document###"                                   +
                        query.value(0).toString()                       + net::Package::delimiter() +
                        QFileInfo(path).fileName()                      + net::Package::delimiter() +
                        query.value(1).toDateTime().toString()          + net::Package::delimiter() +
                        DocumentSerializer::toByte(path));
        }
    }
    qDebug() << "Message history has size:" << messages.size();

    query.finish();

    query.prepare("UPDATE messages SET isRead = TRUE "
                  "WHERE (isRead = FALSE) AND (id_conversation = :id)");
    query.bindValue(":id", conversation_id);

    if(!query.exec()) {
        qDebug() << Q_FUNC_INFO << "Can't update messages!";
    }
    return messages;
}


bool DBFacade::newConversation(const QString &user1, const QString &user2)
{
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO conversations(title, id_creator) "
                  "SELECT CONCAT(:user1, \"$$$\", :user2), :null "
                  "WHERE"
                  "("
                  "SELECT COUNT(*) "
                  "FROM conversations "
                  "WHERE "
                  "(title = CONCAT(:user1, \"$$$\", :user2)) "
                  "OR "
                  "(title = CONCAT(:user2, \"$$$\", :user1))"
                  ") = 0 "
                  "AND "
                  "(:user1 IN (SELECT email FROM useres)) "
                  "AND "
                  "(:user2 IN (SELECT email FROM useres)) ");
    query.bindValue(":user1", user1);
    query.bindValue(":user2",user2);
    query.bindValue(":null", QVariant());

    bool ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << "Cannot insert to conversations" << query.lastError().text();
        return false;
    }
    auto queryResultId = query.lastInsertId();
    if(!queryResultId.isValid()) {
        return false;
    }
    int conversationId = queryResultId.toInt();
    query.finish();
    const QString participatsIncert =
            "INSERT INTO participants (id_conversation, id_useres) "
            "SELECT :conversationId, u.id "
            "FROM useres u "
            "WHERE u.email = :email ";
    query.prepare(participatsIncert);
    query.bindValue(":conversationId", conversationId);
    query.bindValue(":email", user1);
    ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << "Cannot insert to parrticipants" << query.lastError().text();
        return false;
    }
    query.finish();
    query.prepare(participatsIncert);
    query.bindValue(":conversationId", conversationId);
    query.bindValue(":email", user2);
    ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << "Cannot insert to parrticipants";
        return false;
    }
    return true;

}

bool DBFacade::createConversation(const QString &title, const QString &creator_email, QStringList users, QString path)
{
    QSqlQuery query(*m_db);
    QString selectUser = "SELECT id FROM useres WHERE email = :email";
    query.prepare(selectUser);
    query.bindValue(":email",creator_email);

    if(!query.exec()){
        qDebug() << Q_FUNC_INFO << " Can't find user with email: " << creator_email;
        return false;
    }
    query.next();
    int id = query.value(0).toInt();
    query.finish();

    query.prepare("INSERT INTO conversation (id_pictures, title, id_creator) VALUES(:path, :title, :id)");
    query.bindValue(":path", path);
    query.bindValue(":title",title);
    query.bindValue(":id",id);

    if(!query.exec()){
        qDebug() << Q_FUNC_INFO << " Can't create conversation with id_creator: " << id << " and title " << title;
        return false;
    }
    qDebug() << Q_FUNC_INFO << "conversation creared!";
    int conversation_id = query.lastInsertId().toInt();
    query.finish();

    addUserToConversation(conversation_id, id);

    foreach(QString user, users)
    {
         int user_id = userID(user);
         if(user_id == -1)
             continue;
         addUserToConversation(conversation_id, user_id);
    }
    return true;
}

QString DBFacade::getConversationData(const int &conversation_id)
{
    //title+AvatarPath+creatorEmail+UserList
    QString data;
    QSqlQuery query(*m_db);

    query.prepare("SELECT title, a.url, u.email "
                  "INNER JOIN attachments a ON conversation.id_picture = a.id "
                  "INNER JOIN useres u ON conversation.id_creator = useres.id "
                  "WHERE conversation.id = :id");
    query.bindValue(":id",conversation_id);

    if(!query.exec())
    {
        qDebug() << Q_FUNC_INFO << "Can't find conversation with id: " << conversation_id;
        return QString();
    }
    query.next();
    data =      query.value(0).toString() +
                net::Package::delimiter() +
                query.value(1).toString() +
                net::Package::delimiter() +
                query.value(2).toString();//title$$$AvatarPath$$$creatorEmail$$$size$$$userdata

    query.finish();

    query.prepare("SELECT u.nickname, u.email, a.url"
                  "INNER JOIN useres u ON partisipans.id_useres = useres.id "
                  "INNER JOIN attachments a ON useres.id_pictures = a.id "
                  "id_conversation = :id");
    query.bindValue(":id",conversation_id);

    if(!query.exec())
    {
        qDebug() << Q_FUNC_INFO << "Can't find partisipants from conversation with id: " << conversation_id;
        return QString();
    }
    data += QString().number(query.size());
    while(query.next())
    {
        data +=     query.value(0).toString() + //email$$$nickname$$$avataUrl
                    net::Package::delimiter() +
                    query.value(1).toString() +
                    net::Package::delimiter() +
                    ImageSerializer::toBase64(query.value(2).toString());
    }
    return data;
}

bool DBFacade::addUserToConversation(const int &conversation_id, const int &user_id)
{
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO participants (id_conversation,id_useres) VALUES(:conversation,:user)");
    query.bindValue(":conversation",conversation_id);
    query.bindValue(":user",user_id);
    if(!query.exec()){
        qDebug() << Q_FUNC_INFO << " Can't add user to conversation, conversation_id = " << conversation_id
                 << " user_id = " << user_id;
        return false;
    }
    query.finish();
    return true;
}

int DBFacade::userID(const QString &user)
{
    QSqlQuery query(*m_db);
    query.prepare(user);
    query.bindValue(":email",user);
    if(!query.exec())
    {
        qDebug() << Q_FUNC_INFO << " Can't find user with email: " << user;
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

QString DBFacade::userNickname(QString email)
{
    QSqlQuery query(*m_db);

    query.prepare("SELECT nickname "
                  "FROM useres "
                  "WHERE email = :email");

    qDebug() << Q_FUNC_INFO << "Getting username for " << email;
    query.bindValue(":email", email);
    bool ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << "Cannot get username" << query.lastError().text();
    }
    if(query.next()) {
        return query.value(0).toString();
    } else {
        return {};
    }
}

QString DBFacade::userImage(QString email)
{
    QSqlQuery query(*m_db);

    query.prepare("SELECT attachments.url "
                  "FROM attachments "
                  "INNER JOIN useres "
                  "ON useres.id_pictures = attachments.id "
                  "WHERE useres.email = :email");

    qDebug() << Q_FUNC_INFO << "Getting url for " << email;
    query.bindValue(":email", email);
    bool ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << "Cannot get user url" << query.lastError().text();
    }
    if(query.next()) {
        return query.value(0).toString();
    } else {
        return {};
    }
}

void DBFacade::newMessage(const QString &sender, const QStringList &recievers, const QString &text, const QString &dateTime)
{

    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO messages (id_sender, id_conversation, message,message_type, created_at, deleted_at) "
                  "SELECT u.id, c.id, :text,:type , :datetime, :deletedat "
                  "FROM useres u "
                  "INNER JOIN participants p ON u.id = p.id_useres "
                  "INNER JOIN conversations c ON p.id_conversation = c.id "
                  "WHERE (u.email = :sender) "
                  "AND "
                  "( "
                  "(c.title LIKE CONCAT(:receiver, '$$$', :sender)) "
                  "OR "
                  "(c.title LIKE CONCAT(:sender, '$$$', :receiver)) "
                  ") "
                  );
    query.bindValue(":text", text);
    query.bindValue(":type","text");
    query.bindValue(":datetime", QDateTime::fromString(dateTime));
    query.bindValue(":deletedat", QVariant());
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", recievers.first());
    qDebug() << text << sender /*<< recievers.first()*/;
    bool ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << query.lastError().number();
    }
}

void DBFacade::newImage(const QString &sender, const QStringList &recievers,
                        const QString& filename, const QByteArray &imageb,
                       const QString &dateTime)
{


    QSqlQuery query(*m_db);
        query.prepare("INSERT INTO messages (id_sender, id_conversation, message_type, created_at, deleted_at) "
                      "SELECT u.id, c.id,:type , :datetime, :deletedat "
                      "FROM useres u "
                      "INNER JOIN participants p ON u.id = p.id_useres "
                      "INNER JOIN conversations c ON p.id_conversation = c.id "
                      "WHERE (u.email = :sender) "
                      "AND "
                      "( "
                      "(c.title LIKE CONCAT(:receiver, '$$$', :sender)) "
                      "OR "
                      "(c.title LIKE CONCAT(:sender, '$$$', :receiver)) "
                      ") "
                      );
        query.bindValue(":type","image");
        query.bindValue(":datetime", QDateTime::fromString(dateTime));
        query.bindValue(":deletedat", QVariant());
        query.bindValue(":sender", sender);
        query.bindValue(":receiver", recievers.first());
        bool ok = query.exec();
        if(!ok) {
            qWarning() << Q_FUNC_INFO << query.lastError().text();
        }

        QVariant id = query.lastInsertId();

        query.finish();



        qDebug() << "image in message column  has id:" << id.toInt();

        QString path = QDir::currentPath() + QDir::separator() + "downloads" + QDir::separator() +
                QUuid::createUuid().toString().remove('{').remove('}') + "_" + filename;

        ImageSerializer::fromBase64(imageb,path);

        query.prepare("INSERT INTO attachments  (id_message,url) VALUES(:id, :url);");

        query.bindValue(":url", path);
        query.bindValue(":id",id.toInt());

        ok = query.exec();
        if(!ok) {
            qWarning() << Q_FUNC_INFO << query.lastError().text();
        }
}

void DBFacade::newDocument(const QString &sender, const QStringList &recievers,
                           const QString &filename, const QByteArray &document,
                           const QString &dateTime)
{
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO messages (id_sender, id_conversation, message_type, created_at, deleted_at) "
                  "SELECT u.id, c.id,:type , :datetime, :deletedat "
                  "FROM useres u "
                  "INNER JOIN participants p ON u.id = p.id_useres "
                  "INNER JOIN conversations c ON p.id_conversation = c.id "
                  "WHERE (u.email = :sender) "
                  "AND "
                  "( "
                  "(c.title LIKE CONCAT(:receiver, '$$$', :sender)) "
                  "OR "
                  "(c.title LIKE CONCAT(:sender, '$$$', :receiver)) "
                  ") "
                  );
    query.bindValue(":type","document");
    query.bindValue(":datetime", QDateTime::fromString(dateTime));
    query.bindValue(":deletedat", QVariant());
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", recievers.first());
    bool ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << query.lastError().text();
    }

    QVariant id = query.lastInsertId();

    query.finish();



    qDebug() << "file in message column  has id:" << id.toInt();

    QString path = QDir::currentPath() + QDir::separator() + "downloads" + QDir::separator() +
            QUuid::createUuid().toString().remove('{').remove('}') + "+" + filename;

    DocumentSerializer::fromByte(document,path);

    query.prepare("INSERT INTO attachments  (id_message,url) VALUES(:id, :url);");

    query.bindValue(":url", path);
    query.bindValue(":id",id.toInt());

    ok = query.exec();
    if(!ok) {
        qWarning() << Q_FUNC_INFO << query.lastError().text();
    }
}

QHash<QString, QString> DBFacade::contactsList(QString email)
{
    QSqlQuery query(*m_db);

    //    QString queryString =
    //            "SELECT u.nickname, a.url "
    //            "FROM useres u "
    //            "INNER JOIN attachments a ON a.id = u.id_pictures "
    //            "INNER JOIN participants p ON u.id = p.id_useres "
    //            "INNER JOIN conversations c ON p.id_conversation = c.id "
    //            "WHERE u.nickname != :username";
//    , HasUnreadMessages(:email, u.email)
    QString queryString =
            "SELECT DISTINCT u.email, a.url, HasUnreadMessages(:email, u.email) "
            "FROM useres u "
            "INNER JOIN attachments a ON a.id = u.id_pictures "
            "INNER JOIN participants p ON u.id = p.id_useres "
            "INNER JOIN conversations c ON p.id_conversation = c.id "
            "WHERE (u.email != :email "
            "AND c.id IN ( "
                "SELECT c2.id "
                "FROM conversations c2 "
                "INNER JOIN participants p2 ON c2.id = p2.id_conversation "
                "INNER JOIN useres u2 ON p2.id_useres = u2.id "
                "WHERE u2.email = :email "
                "))";
    query.prepare(queryString);
    query.bindValue(":email", email);
    bool ok = query.exec();
    if(!ok) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
    }
    qDebug() << query.size();
    QHash<QString, QString> contacts;
    while(query.next()) {
        QString contactName = query.value(0).toString();
        QString url = query.value(1).toString();
        bool hasUnread = query.value(2).toBool();
        qDebug() << email << " has unread = " << hasUnread << "from" << contactName;
        QString key = contactName + net::Package::delimiter() + QString::number(hasUnread);
        qDebug() << key;
        contacts[key] = url;
    }
    return contacts;
}

QList<int> DBFacade::conversationList(QString user)
{
    QSqlQuery query(*m_db);
    query.prepare("SELECT id_conversation FROM participants p "
                  "INNER JOIN useres u ON p.id_useres = u.id "
                  "WHERE u.email = :user AND p.id_creator IS NOT NULL");
    query.bindValue(":user",user);

    bool ok = query.exec();
    if(!ok) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
    }
    QList<int> rezult;
    qDebug() << query.size();
    while (query.next())
        rezult.append(query.value(0).toInt());
    return rezult;
}

QSqlDatabase *DBFacade::db() const
{
    return m_db;
}

void DBFacade::setDb(QSqlDatabase *db)
{
    m_db = db;
}
