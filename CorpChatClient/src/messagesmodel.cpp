#include "../include/messagesmodel.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MessagesModel::MessagesModel(QObject *parent)
    : QAbstractListModel(parent),
      m_list(nullptr)
{

}

int MessagesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->items().size();
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_list)
        return QVariant();

    qDebug() << "Getting data at: " << index.row() << " with role " << role;
    const Message &item = m_list->items().at(index.row());

    switch (role)
    {
    case SenderRole:
        return QVariant(item.sender);

    case TextRole:
        return QVariant(item.data);

    case TimeRole:
        return QVariant(item.timeStamp);

    case ImageRole:
        return QVariant(item.imageurl);

    case DocumentRole:
        return QVariant(item.documenturl);
    }


    return QVariant();
}

bool MessagesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || !m_list)
    {
        qDebug() << "setData error: invalid index";
        return false;
    }
    Message item = m_list->items().at(index.row());

    switch (role) {
    case SenderRole:
        item.sender = value.toString();
        break;
    case TextRole:
        item.data = value.toString();
        break;
    case TimeRole:
        item.timeStamp = value.toString();
        break;
    case ImageRole:
        item.imageurl = value.toString();
        break;
    case DocumentRole:
        item.documenturl = value.toString();
        break;
    }

    if(m_list->setItemAt(index.row(), item))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    qDebug() << "cannot setItemAt in list";
    return false;
}

bool MessagesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    if(!m_list)
        return false;

    beginInsertRows(QModelIndex(), row, row+count-1);

    for(int i = 0; i < count; ++i)
    {
        qDebug() << "insertRows: invoked inner insert";
        m_list->items().insert(row,Message{});
    }
    qDebug() << "insertRows: row count: " << m_list->items().size();

    endInsertRows();
    return true;
}

Qt::ItemFlags MessagesModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QHash<int, QByteArray> MessagesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[SenderRole] = "nickname";
    roles[TextRole] = "text";
    roles[TimeRole] = "timeStamp";
    roles[ImageRole] = "image";
    roles[DocumentRole] = "document";

    return roles;
}

void MessagesModel::reset()
{
    beginResetModel();

    m_list->items().clear();
    QAbstractListModel::resetInternalData();

    endResetModel();
}

MessagesList *MessagesModel::list() const
{
    return m_list;
}

void MessagesModel::setList(MessagesList *list)
{
    beginResetModel();
    if(m_list)
        m_list->disconnect(this);

    m_list = list;
    if(m_list)
    {
        connect(m_list, &MessagesList::preItemAppended, this, [=](){
            const int index = m_list->items().size();
            beginInsertRows(QModelIndex(),index,index);
        });
        connect(m_list, &MessagesList::postItemAppended, this, [=](){
            endInsertRows();
        });

        connect(m_list, &MessagesList::preItemRemoved, this, [=](int index){
            beginRemoveRows(QModelIndex(),index,index);
        });
        connect(m_list, &MessagesList::postItemRemoved, this, [=](){
            endRemoveRows();
        });
    }
    endResetModel();
}

void MessagesModel::append(const Message &item)
{
    if(insertRows(0,1,{}))
        qDebug() << "Appending at " << this->rowCount({});

    if(this->rowCount({}) == 0)
    {
        qDebug() << "fuck! model is empty!";
    }
    bool ok = 0;

    qDebug() << item.sender << item.data;
    //    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.sender), Roles::SenderRole);
    //    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.data), Roles::TextRole);
    //    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.timeStamp), Roles::TimeRole);

    ok |= setData(this->index(0), QVariant(item.sender), Roles::SenderRole);
    ok |= setData(this->index(0), QVariant(item.data), Roles::TextRole);
    ok |= setData(this->index(0), QVariant(item.timeStamp), Roles::TimeRole);
    ok |= setData(this->index(0), QVariant(item.imageurl), Roles::ImageRole);
    ok |= setData(this->index(0), QVariant(item.documenturl), Roles::DocumentRole);

    if(!ok)
        qDebug() << "Set data not working!";
}
