#ifndef IMAGESERIALIZER_H
#define IMAGESERIALIZER_H

#include <QObject>
#include <QBuffer>
#include <QByteArray>
#include <QImage>
#include <QStringView>
#include <QDebug>
#include <QFile>
#include <QDir>
#include<QFileInfo>
#include "package.h"

class ImageSerializer
{
public:
    ImageSerializer() = delete;
    ImageSerializer(const ImageSerializer&) = delete;
    ImageSerializer(ImageSerializer&&) = delete;
    ~ImageSerializer() = delete;

    static QByteArray toBase64(const QString &path)
    {
        QImage avatar;
        QFileInfo fi(path);
        QString ext = fi.suffix();  // ext = "gz"
        bool ok = avatar.load(path, ext.toStdString().c_str());
        if(!ok)
        {
            qDebug() << Q_FUNC_INFO << "Can't load image from " << path;
        }
        QByteArray imgRaw;         
        QBuffer buff(&imgRaw);

        buff.open(QIODevice::WriteOnly | QIODevice::Truncate);
        ok = avatar.save(&buff, ext.toStdString().c_str());
        if(!ok)
        {
            qDebug() << "Can't save image to buffer";
        }
        buff.close();

        QString result = ext + "%%%" + imgRaw.toBase64();
        return result.toUtf8();
    }

    static void fromBase64(QByteArray raw1, const QString &path)
    {
        QString unspletedraw(std::move(raw1));
        QStringList temp = unspletedraw.split("%%%");
        qDebug() << Q_FUNC_INFO << "String was splited, her size will be:" << temp.size() << " format = " << temp.first();
        QString format = temp.first();
        QByteArray raw = temp.last().toUtf8();

        QImage avatar;
        QByteArray imgRaw = QByteArray::fromBase64(raw);
        QDir dir(QDir::currentPath());

        if(!QDir(QDir::currentPath() + QDir::separator() + "images").exists())
            if(!dir.mkdir("images"))
                qFatal("Can't create sub-dir!!!");


        QFile file{path};
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qDebug() << Q_FUNC_INFO << "Can't create file format = " << format << ", path = " << path;
            return;
        }
        if(!avatar.loadFromData(imgRaw, format.toStdString().c_str()))
        {
            qDebug() << Q_FUNC_INFO << " Can't load image from base64 format = " << format;
            return;
        }
        if(!avatar.save(&file, format.toStdString().c_str()))
        {
            qDebug() << Q_FUNC_INFO << " Can't save image to file format = " << format;
            return;
        }
        file.close();
    }
};


#endif // IMAGESERIALIZER_H
