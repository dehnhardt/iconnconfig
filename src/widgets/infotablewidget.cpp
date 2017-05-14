#include "infotablewidget.h"

#include <QGridLayout>
#include <QHeaderView>

InfoTableWidget::InfoTableWidget(
    QWidget *parent,
    std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *retSetInfos)
    : QWidget(parent), retSetInfos(retSetInfos) {
  QGridLayout *lo = new QGridLayout();
  QPalette qp;
  setLayout(lo);
  int i = 0;
  if (this->retSetInfos) {
    tw = new QTableWidget(retSetInfos->size(), 3, this);
    tw->setColumnHidden(2, true);
    setupTable();
    for (std::map<SysExMessage::DeviceInfoItem, RetSetInfo *>::iterator it =
             retSetInfos->begin();
         it != retSetInfos->end(); ++it) {
      SysExMessage::DeviceInfoItem infoItem = it->first;
      RetSetInfo *info = it->second;
      QTableWidgetItem *name =
          new QTableWidgetItem(info->getItemName().c_str());
      name->setForeground(qp.dark());
      QTableWidgetItem *value = new QTableWidgetItem(info->getValue().c_str());
      name->setFlags(name->flags() & ~Qt::ItemIsEditable);
      QTableWidgetItem *itemType =
          new QTableWidgetItem(QString::number((int)infoItem));
      if (!info->isItemEditable()) {
        value->setFlags(value->flags() & ~Qt::ItemIsEditable);
        value->setForeground(qp.dark());
      }
      tw->setItem(i, 0, name);
      tw->setItem(i, 1, value);
      tw->setItem(i, 2, itemType);
      ++i;
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
    val = tw->item(row, column)->text().toStdString();
    i = tw->item(row, 2)->text().toInt();
    SysExMessage::DeviceInfoItem item = (SysExMessage::DeviceInfoItem)i;
    if (i > 0)
      emit deviceInfoChanged(item, val);
  }
}
