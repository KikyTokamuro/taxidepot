#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <tabledelegate.h>

#include "dbmodels.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_addNewTaxistButton_clicked();
    void on_updateTaxistsButton_clicked();
    void on_deleteTaxistButton_clicked();
    void on_createOrderButton_clicked();
    void on_updateOrdersButton_clicked();
    void on_orderCompleteButton_clicked();
    void on_orderCancelButton_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    void connectModelsToUI();
    void tableSettings();
    void drawOrdersChart();

    Ui::MainWindow *ui;
    DbModels *dbmodels;
    TableDelegate *m_delegate;
};
#endif // MAINWINDOW_H
