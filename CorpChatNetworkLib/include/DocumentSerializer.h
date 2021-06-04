#ifndef DOCUMENTSERIALIZER_H
#define DOCUMENTSERIALIZER_H
#include <QObject>
#include <QBuffer>
#include <QByteArray>
#include <QImage>
#include <QStringView>
#include <QDebug>
#include <QFile>
#include <QDir>
#include<QFileInfo>

class DocumentSerializer
{
    DocumentSerializer() = delete;
    DocumentSerializer(const DocumentSerializer&) = delete;
    DocumentSerializer(DocumentSerializer&&) = delete;
    ~DocumentSerializer() = delete;
public:

    static void fromByte(const QByteArray& raw,const QString& path)
    {

        QDir dir(QDir::currentPath());

        if(!QDir(QDir::currentPath() + QDir::separator() + "downloads").exists())
            if(!dir.mkdir("downloads"))
                qFatal("Can't create sub-dir!!!");


        QFile file{path};
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qDebug() << Q_FUNC_INFO << "Can't create file! path: " << path << " File name :" << file.fileName();

            return;
        }
        file.write(QByteArray::fromBase64(raw));
        file.close();
    }

    static QByteArray toByte(const QString& path)
    {
        QFile file(path);

        if(file.open(QIODevice::ReadOnly))
            qDebug() << Q_FUNC_INFO << "Cant open file! path: " << path;

        QByteArray raw = file.readAll().toBase64();
        qDebug() << "document size: " << raw.size();

        return raw;
    }
};


#endif // DOCUMENTSERIALIZER_H
