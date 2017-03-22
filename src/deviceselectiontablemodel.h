#ifndef DEVICESELECTIONTABLEMODEL_H
#define DEVICESELECTIONTABLEMODEL_H

#include "config/configuration.h"
#include <QAbstractTableModel>

class DeviceSelectionTableModel : public QAbstractTableModel {
public:
  DeviceSelectionTableModel(Devices *devices);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;

private:
  std::vector<Device *> tableData;
};

#endif // DEVICESELECTIONTABLEMODEL_H
