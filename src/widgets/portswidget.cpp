#include "portswidget.h"

#include <QLabel>

PortsWidget::PortsWidget(MioMain *parent, Device *device, QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
  infoSections = std::vector<std::string>{"DIN", "USB-Host", "ETH"};
}

PortsWidget::~PortsWidget() {}

QWidget *PortsWidget::createWidget(std::string infoName) {
  QWidget *w = new QWidget(this->parentWidget());
  QGridLayout *lo = new QGridLayout();
  w->setLayout(lo);
  QLabel *l = new QLabel(w);
  l->setText(QString::fromStdString(infoName));
  lo->addWidget(l, 0, 0);
  return w;
}
