#ifndef USERSLIST_H
#define USERSLIST_H
#include<QObject>

struct User
{
    QString nickname;
    QString email;
    QString imgUrl;
    bool ban;

    bool operator==(const User& other)
    {return this->email == other.email;}
};

class UsersList: public QObject
{
    Q_OBJECT
public:
    explicit UsersList(QObject *parent = nullptr);

    bool setItemAt(int index, const User &item);

    bool exists(const User &item) const;
    bool exists(const QString &email) const;

    QList<User> &items();

    void clear();
signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
private:
    QList<User> m_items;
};


#endif // USERSLIST_H
