#ifndef ICONNECTION_H
#define ICONNECTION_H
#include "CorpChatNetworkLib_global.h"
/*
 * Возмжно, будет использовано при смене типов соединения.
 * На данный момент - НЕ ИСПОЛЬЗОВАТЬ.
 */
namespace net
{

class IConnection
{
    virtual void setDescriptor(qintptr descriptor) = 0;
    virtual void write(QByteArray data) = 0;
    virtual QByteArray read(bool *ok) = 0;
    virtual ~IConnection() = 0;
};

}
#endif // ICONNECTION_H
