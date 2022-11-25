#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <qstring.h>

class DbManager
{
public:
    DbManager(const QString& path);
    ~DbManager();
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
