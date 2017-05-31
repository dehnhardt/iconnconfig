#include "portroutingwidget.h"

#include <sstream>

PortRoutingWidget::PortRoutingWidget(QWidget *parent, int numDinPorts)
    : QWidget(parent), numDinPorts(numDinPorts) {
  createWidgets();
}

void PortRoutingWidget::createWidgets() {
  layout = new QGridLayout(this);
  for (int i = 0; i < numDinPorts; ++i) {
    std::stringstream s;
    s << (i + 1);
		PortButton *b = new PortButton(i, QString(s.str().c_str()), DIN, this);
    layout->addWidget(b, 0, i);
  }
  for (int i = 0; i < 10; ++i) {
    std::stringstream s;
    s << (i + 1);
		PortButton *b =
				new PortButton(i, QString(s.str().c_str()), USB_DEVICE, this);
    layout->addWidget(b, 1, i);
  }
  for (int i = 0; i < 5; ++i) {
    std::stringstream s;
    s << (i + 1);
		PortButton *b = new PortButton(i, QString(s.str().c_str()), ETHERNET, this);
    layout->addWidget(b, 2, i);
  }
}

void PortRoutingWidget::setupWidgets() {}

void PortRoutingWidget::setupLayout() { setLayout(layout); }

void PortRoutingWidget::setData() {}

void PortRoutingWidget::createConnections() {}
