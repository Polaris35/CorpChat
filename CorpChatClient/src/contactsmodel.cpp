#include "../include/contactsmodel.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

void ContactsModel::indexChanged(int idx)
{
    qDebug() << Q_FUNC_INFO << "selected index = " << idx;
    m_currentIndex = idx;
    emit selectedChanged(m_currentIndex);
}

void ContactsModel::indexChangedGroup(int idx)
{
    qDebug() << Q_FUNC_INFO << "selected index = " << idx;
    m_currentIndex = idx;
    emit selectedChangedGroup(m_currentIndex);
}

int ContactsModel::currentIndex() const
{
    return m_currentIndex;
}

QString ContactsModel::currentDialog() const
{
    qDebug() << currentIndex();
    return data(this->index(currentIndex()),Roles::EmailRole).toString();
}

QString ContactsModel::currentAvatar() const
{
    return data(this->index(currentIndex()),Roles::ImageRole).toString();
}

QString ContactsModel::currentType() const
{
    return data(this->index(currentIndex()),Roles::TypeRole).toString();
}



ContactsModel::ContactsModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_list(nullptr)
{

}

int ContactsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->items().size();
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_list)
        return QVariant();

    qDebug() << "Getting data at: " << index.row() << " with role " << role;
    const Contact &item = m_list->items().at(index.row());

    switch (role) {

    case EmailRole:
        return QVariant(item.email);

    case NicknameRole:
        return QVariant(item.nickname);

    case ImageRole:
        return QVariant(item.imageUrl);

    case TypeRole:
        return QVariant(item.type);

    case CreatorRole:
        return QVariant(item.creator_email);

    case UserListRole:
        return QVariant(QVariant::fromValue<QList<ContactData>>(item.users));
    }


    return QVariant();
}

bool ContactsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || !m_list)
    {
        qDebug() << "setData error: invalid index";
        return false;
    }
    Contact item = m_list->items().at(index.row());

    switch (role) {
    case EmailRole:
        item.email = value.toString();
        break;
    case NicknameRole:
        item.nickname = value.toString();
        break;
    case ImageRole:
        item.imageUrl = value.toString();
        break;
    case TypeRole:
        item.type = value.toString();
        break;
    case CreatorRole:
        item.creator_email = value.toString();
        break;
    case UserListRole:
        item.users = value.value<QList<ContactData>>();
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

bool ContactsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    if(!m_list)
        return false;

    beginInsertRows(QModelIndex(), row, row+count-1);

    for(int i = 0; i < count; ++i)
    {
        qDebug() << "insertRows: invoked inner insert";
        m_list->items().insert(row,Contact{});
    }
    qDebug() << "insertRows: row count: " << m_list->items().size();

    endInsertRows();
    return true;
}

Qt::ItemFlags ContactsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ContactsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[EmailRole] = "email";
    roles[NicknameRole] = "nickname";
    roles[ImageRole] = "avatar";
    roles[TypeRole] = "type";
    roles[CreatorRole] = "creator_email";
    roles[UserListRole] = "userlist";
    return roles;
}

ContactsList *ContactsModel::list() const
{
    return m_list;
}

void ContactsModel::setList(ContactsList *list)
{
    beginResetModel();
    if(m_list)
        m_list->disconnect(this);

    m_list = list;
    if(m_list)
    {
        connect(m_list, &ContactsList::preItemAppended, this, [=](){
            const int index = m_list->items().size();
            beginInsertRows(QModelIndex(),index,index);
        });
        connect(m_list, &ContactsList::postItemAppended, this, [=](){
            endInsertRows();
        });

        connect(m_list, &ContactsList::preItemRemoved, this, [=](int index){
            beginRemoveRows(QModelIndex(),index,index);
        });
        connect(m_list, &ContactsList::postItemRemoved, this, [=](){
            endRemoveRows();
        });
    }
    endResetModel();
}

void ContactsModel::append(const Contact &item)
{
    if(insertRows(this->rowCount({}),1,{}))
        qDebug() << "Appending at " << this->rowCount({});

    if(this->rowCount({}) == 0)
    {
        qDebug() << "fuck! model is empty!";
    }
    bool ok = 0;
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.email), Roles::EmailRole);
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.nickname), Roles::NicknameRole);
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.imageUrl), Roles::ImageRole);
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.type), Roles::TypeRole);
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.creator_email), Roles::CreatorRole);
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(QVariant::fromValue<QList<ContactData>>(item.users))
                  , Roles::UserListRole);

    if(!ok)
        qDebug() << "Set data not working!";

}


