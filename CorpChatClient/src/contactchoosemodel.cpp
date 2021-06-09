#include "../include/contactchoosemodel.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

void ContactsChooseModel::indexChanged(int idx)
{
    qDebug() << Q_FUNC_INFO << "selected index = " << idx;
    m_currentIndex = idx;
}

int ContactsChooseModel::currentIndex() const
{
    return m_currentIndex;
}

QString ContactsChooseModel::currentDialog() const
{
    qDebug() << currentIndex();
    return data(this->index(currentIndex()),Roles::EmailRole).toString();
}

QString ContactsChooseModel::currentAvatar() const
{
    return data(this->index(currentIndex()),Roles::ImageRole).toString();
}

ContactsChooseModel::ContactsChooseModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_list(nullptr)
{

}

int ContactsChooseModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_list)
        return 0;

    return m_list->items().size();
}

QVariant ContactsChooseModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_list)
        return QVariant();

    qDebug() << "Getting data at: " << index.row() << " with role " << role;
    const ChooseContact &item = m_list->items().at(index.row());

    switch (role) {

    case EmailRole:
        return QVariant(item.email);

    case NicknameRole:
        return QVariant(item.nickname);

    case ImageRole:
        return QVariant(item.imageUrl);

    case ChooseRole:
        return QVariant(item.isChosen);
    }


    return QVariant();
}

bool ContactsChooseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || !m_list)
    {
        qDebug() << "setData error: invalid index";
        return false;
    }
    ChooseContact item = m_list->items().at(index.row());

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
    case ChooseRole:
        item.isChosen = value.toBool();
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

bool ContactsChooseModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    if(!m_list)
        return false;

    beginInsertRows(QModelIndex(), row, row+count-1);

    for(int i = 0; i < count; ++i)
    {
        qDebug() << "insertRows: invoked inner insert";
        m_list->items().insert(row,ChooseContact{});
    }
    qDebug() << "insertRows: row count: " << m_list->items().size();

    endInsertRows();
    return true;
}

Qt::ItemFlags ContactsChooseModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ContactsChooseModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[EmailRole] = "email";
    roles[NicknameRole] = "nickname";
    roles[ImageRole] = "avatar";
    roles[ChooseRole] = "isChoose";
    return roles;
}

ChooseContactsList *ContactsChooseModel::list() const
{
    return m_list;
}

void ContactsChooseModel::setList(ChooseContactsList *list)
{
    beginResetModel();
    if(m_list)
        m_list->disconnect(this);

    m_list = list;
    if(m_list)
    {
        connect(m_list, &ChooseContactsList::preItemAppended, this, [=](){
            const int index = m_list->items().size();
            beginInsertRows(QModelIndex(),index,index);
        });
        connect(m_list, &ChooseContactsList::postItemAppended, this, [=](){
            endInsertRows();
        });

//        connect(m_list, &ChooseContactsList::preItemRemoved, this, [=](int index){
//            beginRemoveRows(QModelIndex(),index,index);
//        });
        connect(m_list, &ChooseContactsList::postItemRemoved, this, [=](){
            endRemoveRows();
        });
    }
    endResetModel();
}

void ContactsChooseModel::append(const ChooseContact &item)
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
    ok |= setData(this->index(this->rowCount({}) - 1), QVariant(item.isChosen), Roles::ChooseRole);

    if(!ok)
        qDebug() << "Set data not working!";

}


