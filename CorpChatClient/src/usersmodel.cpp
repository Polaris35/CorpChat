#include "../include/usersmodel.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

void UsersModel::indexChanged(int idx)
{
    qDebug() << Q_FUNC_INFO << "selected index = " << idx;
    m_currentIndex = idx;
    emit selectedChanged(m_currentIndex);
}

int UsersModel::currentIndex() const
{
    return m_currentIndex;
}

bool UsersModel::currentBan() const
{
    return data(this->index(currentIndex()),Roles::BanRole).toBool();
}

QString UsersModel::currentNickname() const
{
    qDebug() << currentIndex();
    return data(this->index(currentIndex()),Roles::NicknameRole).toString();
}

QString UsersModel::currentAvatar() const
{
    return data(this->index(currentIndex()),Roles::ImageRole).toString();
}

UsersModel::UsersModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_list(nullptr)
{

}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->items().size();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_list)
        return QVariant();

    qDebug() << "Getting data at: " << index.row() << " with role " << role;
    const User &item = m_list->items().at(index.row());

    switch (role) {

    case EmailRole:
        return QVariant(item.email);

    case NicknameRole:
        return QVariant(item.nickname);

    case ImageRole:
        return QVariant(item.imgUrl);

    case BanRole:
        return QVariant(item.ban);

    }


    return QVariant();
}

bool UsersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || !m_list)
    {
        qDebug() << "setData error: invalid index";
        return false;
    }
    User item = m_list->items().at(index.row());

    switch (role) {
    case EmailRole:
        item.email = value.toString();
        break;
    case NicknameRole:
        item.nickname = value.toString();
        break;
    case ImageRole:
        item.imgUrl = value.toString();
        break;
    case BanRole:
        item.ban = value.toBool();
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

bool UsersModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    if(!m_list)
        return false;

    beginInsertRows(QModelIndex(), row, row+count-1);

    for(int i = 0; i < count; ++i)
    {
        qDebug() << "insertRows: invoked inner insert";
        m_list->items().insert(row,User{});
    }
    qDebug() << "insertRows: row count: " << m_list->items().size();

    endInsertRows();
    return true;
}

Qt::ItemFlags UsersModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QHash<int, QByteArray> UsersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[EmailRole] = "email";
    roles[NicknameRole] = "nickname";
    roles[ImageRole] = "avatar";
    roles[BanRole] = "ban";
    return roles;
}

UsersList *UsersModel::list() const
{
    return m_list;
}

void UsersModel::setList(UsersList *list)
{
    beginResetModel();
    if(m_list)
        m_list->disconnect(this);

    m_list = list;
    if(m_list)
    {
        connect(m_list, &UsersList::preItemAppended, this, [=](){
            const int index = m_list->items().size();
            beginInsertRows(QModelIndex(),index,index);
        });
        connect(m_list, &UsersList::postItemAppended, this, [=](){
            endInsertRows();
        });

        connect(m_list, &UsersList::preItemRemoved, this, [=](int index){
            beginRemoveRows(QModelIndex(),index,index);
        });
        connect(m_list, &UsersList::postItemRemoved, this, [=](){
            endRemoveRows();
        });
    }
    endResetModel();
}

void UsersModel::append(const User &item)
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
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.imgUrl), Roles::ImageRole);
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.ban), Roles::BanRole);

    if(!ok)
        qDebug() << "Set data not working!";

}


