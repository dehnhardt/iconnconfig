#ifndef DEVICESELECTIONTABLEMODEL_H
#define DEVICESELECTIONTABLEMODEL_H

#include "config/configuration.h"
#include <QAbstractTableModel>

class DeviceSelectionTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	DeviceSelectionTableModel(Devices *devices);

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
	std::vector<Device *> m_pTableData;
};

#endif // DEVICESELECTIONTABLEMODEL_H
