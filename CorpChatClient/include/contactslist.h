#ifndef CONTACTSLIST_H
#define CONTACTSLIST_H

#include <QObject>
#include "userdata.h"

struct ContactData
{
public:
    ContactData() = default;
    ContactData(const ContactData&) = default;
    ContactData& operator=(const ContactData&) = default;
    QString username() const;
    void setUsername(const QString &username);

    QString email() const;
    void setEmail(const QString &email);

    QString imageUrl() const;
    void setImageUrl(const QString &imageUrl);
    bool operator==(const ContactData& user);

private:
    QString m_username;
    QString m_email;
    QString m_imageUrl;
};
Q_DECLARE_METATYPE(ContactData)

struct Contact
{
    QString nickname;
    QString email;
    QString imageUrl;
    QString type;
    QString creator_email;
    QList<ContactData> users;

    bool operator==(const Contact& other)
    {return this->email == other.email;}
};


class ContactsList : public QObject
{
    Q_OBJECT
public:
    explicit ContactsList(QObject *parent = nullptr);

    bool setItemAt(int index, const Contact &item);

    bool exists(const Contact &item) const;
    bool exists(const QString &email) const;

    QList<Contact> &items();

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
private:
    QList<Contact> m_items;
};

#endif // CONTACTSLIST_H
