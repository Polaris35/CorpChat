#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include<QSqlError>
#include "server.h"
#include "dbfacade.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("ChatDB");
    db.setUserName("root");
    db.setPassword("");
    Server server;
    bool ok = db.open();
    if(!ok) {
        qInfo() << "Can't connect to database\n" << db.lastError().text();
    } else {
        qInfo() << "Database connected!";

        DBFacade databaseWrapper;
        databaseWrapper.setDb(&db);

        server.setDatabase(&databaseWrapper);

        server.start();
    }

    return a.exec();
}
