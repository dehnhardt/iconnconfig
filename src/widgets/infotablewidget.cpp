#include "infotablewidget.h"

#include <QGridLayout>
#include <QHeaderView>

InfoTableWidget::InfoTableWidget(QWidget *parent, DeviceInfo *deviceInfo)
		: QWidget(parent), deviceInfo(deviceInfo) {
	QGridLayout *lo = new QGridLayout();
	QPalette qp;
	setLayout(lo);
	if (this->deviceInfo) {
		std::vector<InfoItem> *infoItems = this->deviceInfo->getDeviceInfos();
		tw = new QTableWidget(infoItems->size(), 2, this);
		setupTable();
		for (unsigned int i = 0; i < infoItems->size(); i++) {
			InfoItem infoItem = infoItems->at(i);
			QTableWidgetItem *name = new QTableWidgetItem(infoItem.name.c_str());
			name->setForeground(qp.dark());
			QTableWidgetItem *value = new QTableWidgetItem(infoItem.value.c_str());
			name->setFlags(name->flags() & ~Qt::ItemIsEditable);
			if (!infoItem.editable) {
				value->setFlags(value->flags() & ~Qt::ItemIsEditable);
				value->setForeground(qp.dark());
			}
			tw->setItem(i, 0, name);
			tw->setItem(i, 1, value);
		}
		lo->addWidget(tw, 0, 0);
		connect(tw, &QTableWidget::cellChanged, this,
						&InfoTableWidget::onDeviceInfoChanged);
	}
}

void InfoTableWidget::setupTable() {
	tw->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Name")));
	tw->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Value")));
	tw->verticalHeader()->hide();
	tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void InfoTableWidget::onDeviceInfoChanged(int row, int column) {
	std::cout << "InfoTableWidget: Row " << row << " column " << column
						<< std::endl;
	if (column == 1) {
		std::string val;
		val = tw->item(row, column)->text().toStdString();
		std::vector<InfoItem> *infoItems = this->deviceInfo->getDeviceInfos();
		InfoItem item = infoItems->at(row);
		SysExMessage::DeviceInfoItem i = item.infoItem;
		deviceInfoChanged(i, val);
	}
}
