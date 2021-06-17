#include "../include/userslist.h"

UsersList::UsersList(QObject *parent) : QObject(parent)
{

}

bool UsersList::setItemAt(int index, const User &item)
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

bool UsersList::exists(const User &item) const
{
    return m_items.contains(item);
}

bool UsersList::exists(const QString &email) const
{
    foreach(User c, m_items)
    {
        if(c.email == email)
            return true;
    }
    return false;
}

QList<User> &UsersList::items()
{
    return m_items;
}

void UsersList::clear()
{
    m_items = QList<User>();
}

void UsersList::appendItem()
{
    emit preItemAppended();

    m_items.append(User{});

    emit postItemAppended();
}
