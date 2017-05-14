#include "ethernetinfowidget.h"

#include <QGridLayout>
#include <QTableWidget>

EthernetInfoWidget::EthernetInfoWidget(
    QWidget *parent, RetSetEthernetPortInfo *retSetEthernetPortInfo)
    : QWidget(parent), retSetEthernetPortInfo(retSetEthernetPortInfo) {
  QGridLayout *lo = new QGridLayout();
  QPalette qp;
  setLayout(lo);
  if (retSetEthernetPortInfo) {
    QTableWidget *tw = new QTableWidget(2, 2, this);
    lo->addWidget(tw, 0, 0);
  }
}
