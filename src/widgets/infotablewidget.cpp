#include "infotablewidget.h"

#include <QGridLayout>
#include <QHeaderView>

InfoTableWidget::InfoTableWidget(
	QWidget *parent,
	std::map<SysExMessage::DeviceInfoItem, std::shared_ptr<RetSetInfo>>
		*retSetInfos)
	: QWidget(parent), m_pRetSetInfos(retSetInfos) {

	QGridLayout *lo = new QGridLayout();
	QPalette qp;

	setLayout(lo);

	int i = 0;
	if (this->m_pRetSetInfos) {
		setupTable();
		for (std::map<SysExMessage::DeviceInfoItem,
					  std::shared_ptr<RetSetInfo>>::iterator it =
				 retSetInfos->begin();
			 it != retSetInfos->end(); ++it) {
			SysExMessage::DeviceInfoItem infoItem = it->first;

			RetSetInfo *info = it->second.get();

			QTableWidgetItem *name =
				new QTableWidgetItem(info->getItemName().c_str());
			QTableWidgetItem *value =
				new QTableWidgetItem(info->getValue().c_str());
			QTableWidgetItem *itemType = new QTableWidgetItem(
				QString::number(static_cast<int>(infoItem)));

			name->setForeground(qp.dark());
			name->setFlags(name->flags() & ~Qt::ItemIsEditable);

			if (!info->isItemEditable()) {
				value->setFlags(value->flags() & ~Qt::ItemIsEditable);
				value->setForeground(qp.dark());
			}
			m_pTableWidget->setItem(i, 0, name);
			m_pTableWidget->setItem(i, 1, value);
			m_pTableWidget->setItem(i, 2, itemType);
			++i;
		}
		lo->addWidget(m_pTableWidget, 0, 0);
		connect(m_pTableWidget, SIGNAL(cellChanged(int, int)), this,
				SLOT(onDeviceInfoChanged(int, int)));
	}
}

void InfoTableWidget::setupTable() {
	m_pTableWidget =
		new QTableWidget(static_cast<int>(m_pRetSetInfos->size()), 3, this);
	m_pTableWidget->setHorizontalHeaderItem(0,
											new QTableWidgetItem(tr("Name")));
	m_pTableWidget->setHorizontalHeaderItem(1,
											new QTableWidgetItem(tr("Value")));
	m_pTableWidget->verticalHeader()->hide();
	m_pTableWidget->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeMode::ResizeToContents);
	m_pTableWidget->setColumnHidden(2, true);
}

/**
 * @brief InfoTableWidget::onDeviceInfoChanged
 * translate row and coult to item and new value of changed information
 * @param row
 * @param column
 */
void InfoTableWidget::onDeviceInfoChanged(int row, int column) {
	std::cout << "InfoTableWidget: Row " << row << " column " << column
			  << std::endl;
	if (column == 1) {
		std::string val;
		int i = -1;
		val = m_pTableWidget->item(row, column)->text().toStdString();
		i = m_pTableWidget->item(row, 2)->text().toInt();
		SysExMessage::DeviceInfoItem item =
			static_cast<SysExMessage::DeviceInfoItem>(i);
		if (i > 0)
			emit deviceInfoChanged(item, val);
	}
}
