#ifndef CONTACTCHOOSEMODEL_H
#define CONTACTCHOOSEMODEL_H

#include "choosecontactlist.h"
#include <QAbstractListModel>
#include <QObject>
#include <QList>



class ContactsChooseModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        EmailRole = Qt::UserRole + 1,
        NicknameRole,
        ImageRole,
        ChooseRole
    };
    ContactsChooseModel(QObject *parent = nullptr);
    ChooseContactsList *list() const;
    void setList(ChooseContactsList *list);
    void append(const ChooseContact &item);
    int currentIndex() const;
    Q_INVOKABLE QString currentDialog() const;
    Q_INVOKABLE QString currentAvatar() const;
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
private:
    ChooseContactsList *m_list;
    int m_currentIndex;
};


#endif // CONTACTCHOOSEMODEL_H
