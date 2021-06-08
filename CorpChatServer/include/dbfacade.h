#ifndef DBFACADE_H
#define DBFACADE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

class DBFacade : public QObject
{
    Q_OBJECT
public:
    enum FileType : int
    {
        DOCUMENT,
        IMAGE,
        AUDIO
    };
    explicit DBFacade(QObject *parent = nullptr);

    QSqlDatabase *db() const;
    void setDb(QSqlDatabase *db);

	bool registerUser(QString username, QString email, QString password, QString imageUrl);
    bool authorizeUser(QString username, QString password);
    void addMessage(QString sender, QString receiver, QString messageText);
    QStringList messageHistory(QString user1, QString user2);
    QStringList messageHistory(const int& conversation_id);

    bool newConversation(const QString &user1, const QString &user2);
    bool createConversation(const QString& title, const QString& creator_email, QStringList users, QString path);
    QString getConversationData(const int& id);
    bool addUserToConversation(const int& conversation_id, const int& user_id);


    int userID(const QString& user);
    QString userNickname(QString email);
    QString userImage(QString email); //returns avatar Url from db

    void newMessage(const QString &sender,
                    const QStringList &recievers,
                    const QString& text,
                    const QString &dateTime);
    void newImage(const QString &sender,
                 const QStringList &recievers,
                  const QString& filename,
                 const QByteArray& image,
                 const QString &dateTime);
    void newDocument(const QString &sender,
                 const QStringList &recievers,
                  const QString& filename,
                 const QByteArray& image,
                 const QString &dateTime);


    QHash<QString, QString> contactsList(QString user);
    QList<int> conversationList(QString user);

signals:
    void error();
private:
    QSqlDatabase *m_db;
};


#endif // DBFACADE_H
