#ifndef PACKAGE_H
#define PACKAGE_H
#include "CorpChatNetworkLib_global.h"

#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>
namespace net {

class CORPCHATNETWORKLIB_EXPORT Package
{
    Q_GADGET
public:
    enum DataType : int
    {
        REGISTRATION_REQUEST = 0,
        AUTH_REQUEST = 1,
        MESSAGE_HISTORY,
        CONTACTS_LIST,
        USER_DATA,
        TEXT_MESSAGE,
        IMAGE,
        DOCUMENT
    };
    Q_ENUM(DataType)
public:
    Package() = default;
    Package(const Package&) = default;
    Package(Package&&) = default;

    Package& operator=(const Package&) = default;
    Package& operator=(Package&&) = default;
public:
    QString sender() const;
    void setSender(const QString &sender);

    QStringList destinations() const;
    void setDestinations(const QStringList &destinations);

    DataType type() const;
    void setType(const DataType &type);

    QVariant data() const;
    void setData(const QVariant &data);
    void setJsonArr(const QJsonArray &arr);

    static QString delimiter();
private:
    QString m_sender;
    QStringList m_destinations;
    DataType m_type;
    QVariant m_data;
};
}
#endif // PACKAGE_H
