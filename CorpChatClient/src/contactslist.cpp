#include "../include/contactslist.h"

ContactsList::ContactsList(QObject *parent) : QObject(parent)
{

}

bool ContactsList::setItemAt(int index, const Contact &item)
{
    if(index < 0 || index >= m_items.size())
        return false;

    const Contact &oldItem = m_items.at(index);

    if(item.nickname == oldItem.nickname && item.imageUrl == oldItem.imageUrl)
        return false;

    m_items[index] = item;
    return true;
}

bool ContactsList::exists(const Contact &item) const
{
    return m_items.contains(item);
}

bool ContactsList::exists(const QString &username) const
{
    foreach(Contact c, m_items)
    {
        if(c.nickname == username)
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
