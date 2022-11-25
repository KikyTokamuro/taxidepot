#ifndef DBMODELS_H
#define DBMODELS_H

#include <QSqlTableModel>

#include "dbmanager.h"

class DbModels
{
public:
    DbModels();
    ~DbModels();
    void initModels();
    QSqlRecord createNewOrderRecord(QString dateStart, QString placeStart, QString placeEnd,
                                    QString clientFullName, QString clientPhone, QVariant taxistId,
                                    int price);
    QSqlRecord createNewTaxistRecord(QString fullname, QString birthday, QString passport,
                                     QString car, QString nick);
    int countOrdersByStatus(int status);


    QSqlTableModel *taxistsModel, *ordersModel;
private:
    void resetFilters();

    DbManager *dbm;

};

#endif // DBMODELS_H
