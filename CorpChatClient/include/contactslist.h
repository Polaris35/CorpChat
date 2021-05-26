#ifndef CONTACTSLIST_H
#define CONTACTSLIST_H

#include <QObject>
struct Contact
{
    QString nickname;
    QString imageUrl;

    bool operator==(const Contact& other)
    {return this->nickname == other.nickname;}
};

class ContactsList : public QObject
{
    Q_OBJECT
public:
    explicit ContactsList(QObject *parent = nullptr);

    bool setItemAt(int index, const Contact &item);

    bool exists(const Contact &item) const;
    bool exists(const QString &username) const;

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
