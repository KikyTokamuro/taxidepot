#include "dbmodels.h"

#include <QDate>
#include <QSqlRecord>

/**
 * @brief DbModels::DbModels
 */
DbModels::DbModels()
{
    dbm = new DbManager("./taxi.db");
    taxistsModel = new QSqlTableModel();
    ordersModel = new QSqlTableModel();
}

/**
 * @brief DbModels::~DbModels
 */
DbModels::~DbModels()
{
    delete taxistsModel;
    delete ordersModel;
    delete dbm;
}

/**
 * @brief Init QSqlTableModel models
 */
void DbModels::initModels()
{
    // Init taxists model
    taxistsModel->setTable("taxists");
    taxistsModel->select();
    taxistsModel->setHeaderData(0, Qt::Horizontal, "ID");
    taxistsModel->setHeaderData(1, Qt::Horizontal, "ФИО");
    taxistsModel->setHeaderData(2, Qt::Horizontal, "Дата рождения");
    taxistsModel->setHeaderData(3, Qt::Horizontal, "Паспорт");
    taxistsModel->setHeaderData(4, Qt::Horizontal, "Машина");
    taxistsModel->setHeaderData(5, Qt::Horizontal, "Позывной");

    // Init orders model
    ordersModel->setTable("orders");
    ordersModel->select();
    ordersModel->setHeaderData(0, Qt::Horizontal, "ID");
    ordersModel->setHeaderData(1, Qt::Horizontal, "Начало поездки");
    ordersModel->setHeaderData(2, Qt::Horizontal, "Конец поездки");
    ordersModel->setHeaderData(3, Qt::Horizontal, "Откуда");
    ordersModel->setHeaderData(4, Qt::Horizontal, "Куда");
    ordersModel->setHeaderData(5, Qt::Horizontal, "ФИО клиента");
    ordersModel->setHeaderData(6, Qt::Horizontal, "Телефон клиента");
    ordersModel->setHeaderData(7, Qt::Horizontal, "Номер таксиста");
    ordersModel->setHeaderData(8, Qt::Horizontal, "Цена поездки");
    ordersModel->setHeaderData(9, Qt::Horizontal, "Статус");
}

/**
 * @brief Crete new order record
 * @param dateStart
 * @param placeStart
 * @param placeEnd
 * @param clientFullName
 * @param clientPhone
 * @param taxistId
 * @param price
 * @return new QSqlRecord record
 */
QSqlRecord DbModels::createNewOrderRecord(QString dateStart, QString placeStart, QString placeEnd,
                                          QString clientFullName, QString clientPhone, QVariant taxistId,
                                          int price)
{
    QSqlRecord newRecord = ordersModel->record();

    newRecord.setGenerated("id", false);
    newRecord.setValue("date_start", dateStart);
    newRecord.setValue("place_start", placeStart);
    newRecord.setValue("place_end", placeEnd);
    newRecord.setValue("client_name", clientFullName);
    newRecord.setValue("client_phone", clientPhone);
    newRecord.setValue("taxist", taxistId);
    newRecord.setValue("price", price);
    newRecord.setGenerated("status", false);

    return newRecord;
}

/**
 * @brief Create new taxist record
 * @param fullname
 * @param birthday
 * @param passport
 * @param car
 * @param nick
 * @return new QSqlRecord record
 */
QSqlRecord DbModels::createNewTaxistRecord(QString fullname, QString birthday, QString passport,
                                           QString car, QString nick)
{
    QSqlRecord newRecord = taxistsModel->record();

    newRecord.setGenerated("id", false);
    newRecord.setValue("fullname", fullname);
    newRecord.setValue("birthday", birthday);
    newRecord.setValue("passport_info", passport);
    newRecord.setValue("car_info", car);
    newRecord.setValue("nick", nick);

    return newRecord;
}

/**
 * @brief Reset filters in models
 */
void DbModels::resetFilters()
{
    ordersModel->setFilter("");
    taxistsModel->setFilter("");
    ordersModel->select();
    taxistsModel->select();
}

/**
 * @brief Get count of orders by status
 * @param status
 * @return count of orders by status
 */
int DbModels::countOrdersByStatus(int status)
{
    ordersModel->setFilter(QString("status = %1").arg(status));
    ordersModel->select();

    while (ordersModel->canFetchMore())
        ordersModel->fetchMore();

    int count = ordersModel->rowCount();

    resetFilters();

    return count;
}
