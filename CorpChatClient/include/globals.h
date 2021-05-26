#ifndef GLOBALS_H
#define GLOBALS_H
#include <QtGlobal>
#include <QString>
#include <QDir>

namespace Globals
{
    //static const QString imagesPath = "/images";
    Q_GLOBAL_STATIC_WITH_ARGS(QString, imagesPath, {QDir::separator() + QLatin1String("images")})
}


#endif // GLOBALS_H
