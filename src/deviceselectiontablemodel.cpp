#include "deviceselectiontablemodel.h"
#include <QStandardItem>
#include <QString>
#include <QtGui>

DeviceSelectionTableModel::DeviceSelectionTableModel(Devices *devices) {
  for (Devices::iterator it = devices->begin(); it != devices->end(); it++)
    tableData.push_back(it->second);
}

int DeviceSelectionTableModel::rowCount(const QModelIndex &parent) const {
  return tableData.size();
}

int DeviceSelectionTableModel::columnCount(const QModelIndex &parent) const {
  return 4;
}

QVariant DeviceSelectionTableModel::data(const QModelIndex &index,
                                         int role) const {
  if (role == Qt::DisplayRole) {
    Device *current = tableData.at(index.row());
    switch (index.column()) {
    case 0:
      break;
    case 1:
      return QString::number(current->getSerialNumber()->getLongValue());
    case 2:
      return QString(current->getDeviceName().c_str());
    case 3:
      return QString(current->getModelName().c_str());
    default:
      return QVariant::Invalid;
      break;
    }
  }
  if ((role == Qt::CheckStateRole) && (index.column() == 0)) {
    return Qt::Unchecked;
  }
  return QVariant::Invalid;
}

QVariant DeviceSelectionTableModel::headerData(int section,
                                               Qt::Orientation orientation,
                                               int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return QString("Default");
      break;
    case 1:
      return QString("Serial Number");
    case 2:
      return QString("Device Name");
    case 3:
      return QString("Device Model");
    default:
      return QVariant::Invalid;
    }
  }
  return QVariant::Invalid;
}

Qt::ItemFlags DeviceSelectionTableModel::flags(const QModelIndex &index) const {
  if (index.column() == 0) {
    return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
  }
  return QAbstractTableModel::flags(index);
}
