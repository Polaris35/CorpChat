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
    explicit DBFacade(QObject *parent = nullptr);

    QSqlDatabase *db() const;
    void setDb(QSqlDatabase *db);

    bool registerUser(QString username, QString password, QString imageUrl);
    bool authorizeUser(QString username, QString password);
    void addMessage(QString sender, QString receiver, QString messageText);
    QStringList messageHistory(QString user1, QString user2);
    bool newConversation(const QString &user1, const QString &user2);
    QString userImage(QString username); //returns avatar Url from db
    void newMessage(const QString &sender,
                    const QStringList &recievers,
                    const QString& text,
                    const QString &dateTime);


    QHash<QString, QString> contactsList(QString user);

signals:
    void error();
private:
    QSqlDatabase *m_db;
};

#endif // DBFACADE_H
