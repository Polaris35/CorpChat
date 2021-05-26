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

class ImageSerializer
{
public:
    ImageSerializer() = delete;
    ImageSerializer(const ImageSerializer&) = delete;
    ImageSerializer(ImageSerializer&&) = delete;
    ~ImageSerializer() = delete;

    static QByteArray toBase64(const QString &path, const QString &format = "PNG")
    {
        QImage avatar;
        bool ok = avatar.load(path);
        if(!ok)
        {
            qDebug() << Q_FUNC_INFO << "Can't load image from " << path;
        }
        QByteArray imgRaw;
        QBuffer buff(&imgRaw);

        buff.open(QIODevice::WriteOnly | QIODevice::Truncate);
        ok = avatar.save(&buff, format.toStdString().c_str());
        if(!ok)
        {
            qDebug() << "Can't save image to buffer";
        }
        buff.close();
        return imgRaw.toBase64();
    }

    static void fromBase64(const QByteArray &raw, const QString &path, const QString &format = "PNG")
    {

        QImage avatar;
        QByteArray imgRaw = QByteArray::fromBase64(raw);
        QDir dir(QDir::currentPath());

        if(!QDir("images").exists())
            if(!dir.mkdir("images"))
                qFatal("Can't create sub-dir!!!");


        QFile file{path};
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qDebug() << Q_FUNC_INFO << "Can't create file";
            return;
        }
        if(!avatar.loadFromData(imgRaw, format.toStdString().c_str()))
        {
            qDebug() << Q_FUNC_INFO << " Can't load image from base64";
            return;
        }
        if(!avatar.save(&file, format.toStdString().c_str()))
        {
            qDebug() << Q_FUNC_INFO << " Can't save image to file";
            return;
        }
        file.close();
    }
};


#endif // IMAGESERIALIZER_H
