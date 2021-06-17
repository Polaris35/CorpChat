#ifndef USERSMODEL_H
#define USERSMODEL_H
#include <QAbstractListModel>
#include <QObject>
#include <QList>

#include "include/userslist.h"

class UsersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        EmailRole = Qt::UserRole + 1,
        NicknameRole,
        ImageRole,
        BanRole
    };
    UsersModel(QObject *parent = nullptr);
    UsersList *list() const;
    void setList(UsersList *list);
    void append(const User &item);

    int currentIndex() const;   
    Q_INVOKABLE QString currentNickname() const;
    Q_INVOKABLE QString currentAvatar() const;
    Q_INVOKABLE bool currentBan() const;
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;


public slots:
    void indexChanged(int idx);
signals:
    void selectedChanged(int idx);    
private:
    UsersList *m_list;
    int m_currentIndex;
};

#endif // USERSMODEL_H
