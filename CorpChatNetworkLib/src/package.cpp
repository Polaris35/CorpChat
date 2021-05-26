#include "../include/package.h"
namespace net
{

//Package::Package() {}

QString Package::sender() const
{
    return m_sender;
}

void Package::setSender(const QString &sender)
{
    m_sender = sender;
}

QStringList Package::destinations() const
{
    return m_destinations;
}

void Package::setDestinations(const QStringList &destinations)
{
    m_destinations = destinations;
}

Package::DataType Package::type() const
{
    return m_type;
}

void Package::setType(const DataType &type)
{
    m_type = type;
}

QVariant Package::data() const
{
    return m_data;
}

void Package::setData(const QVariant &data)
{
    m_data = data;
}

void Package::setJsonArr(const QJsonArray &arr)
{
    qDebug() << Q_FUNC_INFO;
    QJsonDocument doc;
    doc.setArray(arr);
    m_data = doc.toJson();
}

QString Package::delimiter()
{
    return "$";
}

}
