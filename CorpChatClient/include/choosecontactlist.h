#ifndef CHOOSECONTACTLIST_H
#define CHOOSECONTACTLIST_H
#include<QObject>
#include "contactslist.h"

struct ChooseContact
{
    QString nickname;
    QString email;
    QString imageUrl;
    bool isChosen;

    bool operator==(const ChooseContact& other)
    {return this->email == other.email;}
};

class ChooseContactsList : public QObject
{
    Q_OBJECT
public:
    explicit ChooseContactsList(QObject *parent = nullptr);

    bool setItemAt(int index, const ChooseContact &item);

    bool exists(const ChooseContact &item) const;
    bool exists(const QString &email) const;

    QList<ChooseContact> &items();
    QStringList getCheckedUser();

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
private:
    QList<ChooseContact> m_items;
};


#endif // CHOOSECONTACTLIST_H
