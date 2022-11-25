#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dbmodels(new DbModels)
{
    ui->setupUi(this);

    // Expand tabs
    ui->tabWidget->tabBar()->setExpanding(true);

    // Init table settings
    tableSettings();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete dbmodels;
    delete ui;
}

/**
 * @brief Init table settings
 */
void MainWindow::tableSettings()
{
    // Init dbmodels
    dbmodels->initModels();

    // Connect dbmodels to UI
    connectModelsToUI();

    // Tables settings
    ui->tableViewOrders->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewOrders->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewTaxists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewTaxists->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewOrders->setColumnHidden(9, true);

    // Table delegate
    m_delegate = new TableDelegate;
    ui->tableViewOrders->setItemDelegate(m_delegate);
}

/**
 * @brief Update table with taxists
 */
void MainWindow::on_updateTaxistsButton_clicked()
{
    dbmodels->taxistsModel->select();
}

/**
 * @brief Update table with orders
 */
void MainWindow::on_updateOrdersButton_clicked()
{
    dbmodels->ordersModel->select();
}

/**
 * @brief Connect Models -> UI (TableView)
 */
void MainWindow::connectModelsToUI()
{
    // TableView
    ui->tableViewTaxists->setModel(dbmodels->taxistsModel);
    ui->tableViewOrders->setModel(dbmodels->ordersModel);

    // ComboBox
    ui->taxistComboBox->setModel(dbmodels->taxistsModel);
    ui->taxistComboBox->setModelColumn(1);
}

/**
 * @brief Add new taxists to db
 */
void MainWindow::on_addNewTaxistButton_clicked()
{
    // Check lineEdit fields for not empty
    if (
        ui->fullnameLineEdit->text().isEmpty() ||
        ui->nickLineEdit->text().isEmpty() ||
        ui->passportLineEdit->text().isEmpty() ||
        ui->carLineEdit->text().isEmpty()
    ) {
        QMessageBox::warning(this, "Внимание", "При добавлении нового работника, должны быть заполнены все поля");
        return;
    }

    // Get data
    QString fullname = ui->fullnameLineEdit->text();
    QString nick     = ui->nickLineEdit->text();
    QString passport = ui->passportLineEdit->text();
    QString car      = ui->carLineEdit->text();
    QString   birthday = ui->birthdayDateEdit->date().toString("dd.MM.yyyy");

    // Create new record
    QSqlRecord newRecord = dbmodels->createNewTaxistRecord(fullname, birthday, passport, car, nick);

    // Insert new record to db
    if (dbmodels->taxistsModel->insertRecord(dbmodels->taxistsModel->rowCount(), newRecord)) {
        dbmodels->taxistsModel->select();
        QMessageBox msgBox;
        msgBox.setText("Новый работник успешно добавлен");
        msgBox.exec();

        // Clear inputs
        ui->fullnameLineEdit->clear();
        ui->nickLineEdit->clear();
        ui->passportLineEdit->clear();
        ui->carLineEdit->clear();
        ui->birthdayDateEdit->clear();
    } else {
        QMessageBox::warning(this, "Внимание", "Ошибка добавления нового работника");
    }
}

/**
 * @brief Delete taxist from db
 */
void MainWindow::on_deleteTaxistButton_clicked()
{
    // Get selection from table
    QItemSelectionModel *select = ui->tableViewTaxists->selectionModel();

    // If selected exists
    if (select->hasSelection()) {
        QModelIndexList rowList = select->selectedRows();
        foreach(QModelIndex rowIndex, rowList) {
            dbmodels->taxistsModel->removeRows(rowIndex.row(), 1);
        }
        dbmodels->taxistsModel->submitAll();
        dbmodels->taxistsModel->select();
    }
}

/**
 * @brief Create new order
 */
void MainWindow::on_createOrderButton_clicked()
{
    QString placeStart = ui->placeStartLineEdit->text();
    QString placeEnd = ui->placeEndLineEdit->text();
    QString clientFullName = ui->clientFullnameLineEdit->text();
    QString clientPhone = ui->clientPhoneLineEdit->text();
    int taxist = ui->taxistComboBox->currentIndex();
    int price = ui->priceSpinBox->value();

    // Check lineEdit fields for not empty
    if (
        clientFullName.isEmpty() || clientPhone.isEmpty() ||
        placeStart.isEmpty() || placeEnd.isEmpty() ||
        taxist == -1 || price < 50
    ) {
        QMessageBox::warning(this, "Внимание", "При создании нового заказа, должны быть заполнены все поля");
        return;
    }

    QDateTime currentDate = QDateTime::currentDateTime();
    QString dateStart = currentDate.toString("dd.MM.yyyy hh:mm:ss");
    QVariant taxistId = dbmodels->taxistsModel->index(taxist, 0).data();

    // Create new record
    QSqlRecord newRecord = dbmodels->createNewOrderRecord(dateStart, placeStart, placeEnd,
                                                          clientFullName, clientPhone, taxistId,
                                                          price);

    // Insert new record to db
    if (dbmodels->ordersModel->insertRecord(dbmodels->ordersModel->rowCount(), newRecord)) {
        dbmodels->ordersModel->select();
        QMessageBox msgBox;
        msgBox.setText("Новый заказ успешно добавлен");
        msgBox.exec();

        // Clear inputs
        ui->placeStartLineEdit->clear();
        ui->placeEndLineEdit->clear();
        ui->clientFullnameLineEdit->clear();
        ui->clientPhoneLineEdit->clear();
    } else {
        QMessageBox::warning(this, "Внимание", "Ошибка добавления нового заказа");
    }
}

/**
 * @brief Set selected order completed
 */
void MainWindow::on_orderCompleteButton_clicked()
{
    // Get selection from table
    QItemSelectionModel *select = ui->tableViewOrders->selectionModel();

    // If selected exists
    if (select->hasSelection()) {
        QModelIndexList rowList = select->selectedRows();

        foreach(QModelIndex rowIndex, rowList) {
            QModelIndex statusCell = dbmodels->ordersModel->index(rowIndex.row(), 9);

            if (statusCell.data() == 0) {
                // Set status = 1
                dbmodels->ordersModel->setData(statusCell, 1);

                // Set date_end = current datetime
                QDateTime currentDate = QDateTime::currentDateTime();
                QString dateEnd = currentDate.toString("dd.MM.yyyy hh:mm:ss");
                QModelIndex dateEndCell = dbmodels->ordersModel->index(rowIndex.row(), 2);
                dbmodels->ordersModel->setData(dateEndCell, dateEnd);
            }
        }

        dbmodels->ordersModel->submitAll();
        dbmodels->ordersModel->select();
    }
}

/**
 * @brief Set selected order canceled
 */
void MainWindow::on_orderCancelButton_clicked()
{
    // Get selection from table
    QItemSelectionModel *select = ui->tableViewOrders->selectionModel();

    // If selected exists
    if (select->hasSelection()) {
        QModelIndexList rowList = select->selectedRows();

        foreach(QModelIndex rowIndex, rowList) {
            QModelIndex statusCell = dbmodels->ordersModel->index(rowIndex.row(), 9);

            if (statusCell.data() == 0) {
                // Set status = 2
                dbmodels->ordersModel->setData(statusCell, 2);
            }
        }

        dbmodels->ordersModel->submitAll();
        dbmodels->ordersModel->select();
    }
}

/**
 * @brief Current tab changed
 * @param index
 */
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 2) {
        ui->chartView->chart()->deleteLater();
        drawOrdersChart();
    }
}

/**
 * @brief Draw orders chart
 */
void MainWindow::drawOrdersChart()
{
    QPieSeries *series = new QPieSeries();
    series->append("Выполняются", dbmodels->countOrdersByStatus(0));
    series->append("Завершенные", dbmodels->countOrdersByStatus(1));
    series->append("Отмененные", dbmodels->countOrdersByStatus(2));

    // Slice settings
    for (int i : { 0, 1, 2 }) {
        QPieSlice *slice = series->slices().at(i);
        slice->setLabelPosition(QtCharts::QPieSlice::LabelOutside);
        slice->setLabel(QString("%1: %2 шт.").arg(slice->label()).arg(slice->value()));

        if (i == 0) {
            slice->setBrush(QBrush(QColor(255, 0, 0, 50)));
        } else if (i == 1) {
            slice->setBrush(QBrush(QColor(0, 255, 0, 50)));
        } else {
            slice->setBrush(QBrush(QColor(255, 255, 0, 50)));
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::AllAnimations);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}
