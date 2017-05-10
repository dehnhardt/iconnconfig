#include "deviceselectiontablemodel.h"
#include "config/configuration.h"

#include <QStandardItem>
#include <QString>
#include <QtGui>

DeviceSelectionTableModel::DeviceSelectionTableModel(Devices *devices) {
	for (Devices::iterator it = devices->begin(); it != devices->end(); ++it)
    tableData.push_back(it->second);
}

int DeviceSelectionTableModel::rowCount(const QModelIndex &parent
                                        __attribute__((unused))) const {
  return tableData.size();
}

int DeviceSelectionTableModel::columnCount(const QModelIndex &parent
                                           __attribute__((unused))) const {
  return 3;
}

QVariant DeviceSelectionTableModel::data(const QModelIndex &index,
                                         int role) const {
  if (role == Qt::DisplayRole) {
    Device *current = tableData.at(index.row());
    switch (index.column()) {
    case 0:
      return QString(current->getDeviceName().c_str());
    case 1:
      return QString(current->getModelName().c_str());
    case 2:
      return current->getSerialNumberString() != ""
                 ? QString(current->getSerialNumberString().c_str())
                 : QString::number(current->getSerialNumber()->getLongValue());
    default:
      return QVariant::Invalid;
      break;
    }
  }
  if ((role == Qt::CheckStateRole) && (index.column() == 0)) {
    Device *current = tableData.at(index.row());
    return current->getDefault() ? Qt::Checked : Qt::Unchecked;
  }
  return QVariant::Invalid;
}

bool DeviceSelectionTableModel::setData(const QModelIndex &index,
                                        const QVariant &value, int role) {
  bool success = true;

  if (index.isValid()) {
    Device *current = tableData.at(index.row());
    if (role == Qt::CheckStateRole) {
      if (index.column() == 0) {
        if (current->getDefault()) {
          success = false;
        } else {
          current->setDefault(value.toBool());
          Configuration::getInstance().setDefaultDevice(
              current->getSerialNumber()->getLongValue());
          for (int i = 0; i < tableData.size(); i++) {
            if (i != index.row()) {
              if (tableData.at(i)->getDefault()) {
                tableData.at(i)->setDefault(false);
                QModelIndex upd = createIndex(i, 0);
                emit dataChanged(upd, upd);
              }
            }
          }
          QModelIndex topLeft = index;
          QModelIndex bottomRight = index;
          emit dataChanged(topLeft, bottomRight);
          success = true;
        }
      } else
        success = false;
    }
  }
  return success;
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
      return QString("Device Name");
    case 2:
      return QString("Device Model");
    case 3:
      return QString("Serial Number");
    default:
      return QVariant::Invalid;
    }
  }
  return QVariant::Invalid;
}

Qt::ItemFlags DeviceSelectionTableModel::flags(const QModelIndex &index) const {
  if (index.column() == 0) {
    return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable |
           Qt::ItemIsEditable;
  }
  return QAbstractTableModel::flags(index);
}
