#include "portswidget.h"

#include <QLabel>

PortsWidget::PortsWidget(MioMain *parent, Device *device, QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
  infoSections = new std::vector<MultiInfoListEntry *>();
}

PortsWidget::~PortsWidget() {}

QWidget *PortsWidget::createWidget(MultiInfoListEntry *entry) {
  QWidget *w = new QWidget(this->parentWidget());
  QGridLayout *lo = new QGridLayout();
  w->setLayout(lo);
  QLabel *l = new QLabel(w);
  l->setText(QString::fromStdString(entry->name));
  lo->addWidget(l, 0, 0);
  return w;
}
