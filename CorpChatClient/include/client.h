#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QVariant>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

#include "include/CorpChatNetworkLib_global.h"
#include "include/connection.h"
#include "include/package.h"
#include "include/imageserializer.h"
#include "include/DocumentSerializer.h"
#include "contactchoosemodel.h"
#include "include/usersmodel.h"
//#include "userdata.h"
//#include "contactsmodel.h"

class MessagesModel;
class ContactsModel;
class UserData;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    ContactsModel *contactsModel() const;
    void setContactsModel(ContactsModel *contactsModel);

    MessagesModel *messagesModel() const;
    void setMessagesModel(MessagesModel *messagesModel);

    ContactsChooseModel *contactsChooseModel() const;
    void setContactsChooseModel(ContactsChooseModel *contactsChooseModel);

    UserData *getUser() const;
    void setUser(UserData *user);

    Q_INVOKABLE QString username() const;

    void start();
    void start(QString hostIp, quint64 port);


    UsersModel *getUsersModel() const;
    void setUsersModel(UsersModel *usersModel);

public slots:
    void updateUserData(QString nickname, QString email, QString image, bool ban);
    void addUserToConversation(const QString& conversation_id);

    void registerNewUser(QString username,QString email, QString password, QString imgUrl);
    void authorize(QString email, QString password);

    void sendMessage(QString text);
    void sendImage(QString url);
    void sendDocument(QString url);

    void getUserList();
    void getContactsList();
    void getConversationList();
    void getMessageHistory();
    void getMessageHistoryForGroupChat();

    void loadUserList(const QStringList& json); // получаем список всех пользователей для админки


    void loadContactsList(const QStringList &json);
    void loadConversationList(const QStringList& json);
    void loadMessageHistory(const QStringList &json);
	void loadGroupMessageHistory(QString dest,const QStringList& json);

    void requestContact(QString email);
    void createConversation(QString title,QString path);

    void qmlNotifyUnreadMessage(QString sender);
private slots:
    void packageRecieved(net::Package package);    
signals:
    void registerSuccess();
    void registerFailure();

    void userBaned();
    void authSuccsess();
    void authFailure();
    void authAdmin();

    void notifyMessage(QString sender);
private:


    void addContact(const QString &contactData);
	void newMessage(QString sender, QString destination, QString time,QString text);
    void newMessage(QString raw);
	void newDocument(QString sender, QString destination, QString filename, QString time, QByteArray base64);
    void newDocument(QString raw);
    void newImage(QString raw);
	void newImage(QString sender, QString destination, QString filename, QString time, QByteArray base64);

    void authorize(QString username, QString email, QByteArray base64);
private:
    UserData *m_user;
    net::Connection m_connection;
    ContactsModel *m_contactsModel;
    MessagesModel *m_messagesModel;
    ContactsChooseModel *m_contactsChooseModel;
    UsersModel *m_usersModel;

    QStringList m_toNotify;
};

#endif // CLIENT_H
