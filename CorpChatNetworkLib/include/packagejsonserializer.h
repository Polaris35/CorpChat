#ifndef PACKAGEJSONSERIALIZER_H
#define PACKAGEJSONSERIALIZER_H
#include "CorpChatNetworkLib_global.h"
#include "IPackageSerializer.h"
#include <QObject>
#include <QByteArray>
#include <QMetaEnum>

namespace net
{

class CORPCHATNETWORKLIB_EXPORT PackageJsonSerializer
        : public IPackageSerializer
{
public:
    PackageJsonSerializer();

    // IPackageSerializer interface
public:
    QByteArray toBytes(Package package) override;
    Package fromBytes(QByteArray bytes, bool *ok) override;
    ~PackageJsonSerializer();
private:
    QMetaEnum m_types;
};

}
#endif // PACKAGEJSONSERIALIZER_H
