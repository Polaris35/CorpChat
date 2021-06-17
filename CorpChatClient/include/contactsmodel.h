#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QList>

#include "contactslist.h"

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {		
		EmailRole = Qt::UserRole + 1,
		NicknameRole,
        ImageRole,
        TypeRole,
        CreatorRole,
        UserListRole
    };
    ContactsModel(QObject *parent = nullptr);
    ContactsList *list() const;
    void setList(ContactsList *list);
    void append(const Contact &item);
    int currentIndex() const;
	QString type() const;
    Q_INVOKABLE QString currentNickname() const;
    Q_INVOKABLE QString currentDialog() const;
    Q_INVOKABLE QString currentAvatar() const;
    Q_INVOKABLE QString currentType() const;
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
    void indexChangedGroup(int idx);
signals:
    void selectedChanged(int idx);
    void selectedChangedGroup(int idx);
private:
    ContactsList *m_list;
    int m_currentIndex;
};

#endif // CONTACTSMODEL_H
