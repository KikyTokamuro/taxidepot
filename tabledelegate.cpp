#include "tabledelegate.h"

#include <QAbstractListModel>

/**
 * @brief Delegate for orders table (initStyleOption)
 * @param option
 * @param index
 */
void TableDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    // Set background by status
    QModelIndex status = index.model()->index(index.row(), 9);

    if (status.data() == 0) {
        option->backgroundBrush = QBrush(QColor(255, 0, 0, 50));
    } else if (status.data() == 1) {
        option->backgroundBrush = QBrush(QColor(0, 255, 0, 50));
    } else {
        option->backgroundBrush = QBrush(QColor(255, 255, 0, 50));
    }

    // Set alignment
    option->displayAlignment = Qt::AlignCenter;
}
