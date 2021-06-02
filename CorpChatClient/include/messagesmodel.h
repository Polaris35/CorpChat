#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "messageslist.h"

class MessagesModel : public QAbstractListModel
{
    Q_OBJECT    
public:
    enum Roles
    {
        MessageType = Qt::UserRole + 1,
        SenderRole,
        TextRole,
        TimeRole

    };
    MessagesModel(QObject *parent = nullptr);
    MessagesList *list() const;
    void setList(MessagesList *list);

    void append(const Message &item);
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;
    void reset();
private:
    MessagesList *m_list;
};

#endif // MESSAGESMODEL_H
