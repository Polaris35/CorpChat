#include "../include/messageslist.h"

MessagesList::MessagesList(QObject *parent)
    : QObject(parent)
{

}

bool MessagesList::setItemAt(int index, const Message &item)
{
    if(index < 0 || index >= m_items.size())
        return false;

    const Message &oldItem = m_items.at(index);

    if(item.sender == oldItem.sender && item.data == oldItem.data && item.timeStamp == oldItem.timeStamp)
        return false;

    m_items[index] = item;
    return true;
}

QList<Message> &MessagesList::items()
{
    return m_items;
}

void MessagesList::appendItem()
{
    emit preItemAppended();

    m_items.append(Message{});

    emit postItemAppended();
}
