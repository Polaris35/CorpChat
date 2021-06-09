#include "../include/contactchoosemodel.h"
#include "../include/choosecontactlist.h"

ChooseContactsList::ChooseContactsList(QObject *parent) : QObject(parent)
{

}

bool ChooseContactsList::setItemAt(int index, const ChooseContact &item)
{
    if(index < 0 || index >= m_items.size())
        return false;

    const ChooseContact &oldItem = m_items.at(index);

    if(item.nickname == oldItem.nickname && item.imageUrl == oldItem.imageUrl
            &&item.email == oldItem.email && item.isChosen == oldItem.isChosen)
        return false;

    m_items[index] = item;
    return true;
}

bool ChooseContactsList::exists(const ChooseContact &item) const
{
    return m_items.contains(item);
}

bool ChooseContactsList::exists(const QString &email) const
{
    ChooseContact c;
    foreach(c, m_items)
    {
        if(c.email == email)
            return true;
    }
    return false;
}

QList<ChooseContact> &ChooseContactsList::items()
{
    return m_items;
}

QStringList ChooseContactsList::getCheckedUser()
{
    QStringList rezult;
    for(ChooseContact &item: m_items)
    {
        if(item.isChosen)
        {
            rezult.append(item.email);
            item.isChosen = false;
        }
    }
    return rezult;
}

void ChooseContactsList::appendItem()
{
    emit preItemAppended();

    m_items.append(ChooseContact{});

    emit postItemAppended();
}
