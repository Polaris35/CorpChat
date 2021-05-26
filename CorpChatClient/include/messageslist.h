#ifndef MESSAGESLIST_H
#define MESSAGESLIST_H

#include <QObject>
struct Message
{
    QString sender;
    QString data;
    QString timeStamp;
    QString imageurl;
    QString documenturl;
};

class MessagesList : public QObject
{
    Q_OBJECT
public:
    explicit MessagesList(QObject *parent = nullptr);

    bool setItemAt(int index, const Message &item);

    QList<Message> &items();

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
private:
    QList<Message> m_items;
};

#endif // MESSAGESLIST_H
