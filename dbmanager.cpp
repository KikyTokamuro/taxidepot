#include "dbmanager.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

/**
 * @brief Connect to sqlite db
 * @param path
 */
DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qDebug() << "Error: connection with database fail";
    } else {
        qDebug() << "Database: connection ok";
    }
}

/**
 * @brief Close sqlite connection
 */
DbManager::~DbManager()
{
    this->m_db.close();
}
