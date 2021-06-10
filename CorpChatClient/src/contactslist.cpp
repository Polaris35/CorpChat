#include "../include/contactslist.h"

ContactsList::ContactsList(QObject *parent) : QObject(parent)
{

}

bool ContactsList::setItemAt(int index, const Contact &item)
{
    if(index < 0 || index >= m_items.size())
        return false;

//    const Contact &oldItem = m_items.at(index);

//    if(item.nickname == oldItem.nickname && item.imageUrl == oldItem.imageUrl
//       &&item.email == oldItem.email && item.type == oldItem.type &&
//            item.users == oldItem.users && item.creator_email == oldItem.creator_email)
//        return false;

    m_items[index] = item;
    return true;
}

bool ContactsList::exists(const Contact &item) const
{
    return m_items.contains(item);
}

bool ContactsList::exists(const QString &email) const
{
    foreach(Contact c, m_items)
    {
        if(c.email == email)
            return true;
    }
    return false;
}

QList<Contact> &ContactsList::items()
{
    return m_items;
}

void ContactsList::appendItem()
{
    emit preItemAppended();

    m_items.append(Contact{});

    emit postItemAppended();
}

QString ContactData::username() const
{
    return m_username;
}

void ContactData::setUsername(const QString &username)
{
    m_username = username;
}

QString ContactData::email() const
{
    return m_email;
}

void ContactData::setEmail(const QString &email)
{
    m_email = email;
}

QString ContactData::imageUrl() const
{
    return m_imageUrl;
}

void ContactData::setImageUrl(const QString &imageUrl)
{
    m_imageUrl = imageUrl;
}

bool ContactData::operator==(const ContactData &user)
{
    return user.email() == m_email;
}
