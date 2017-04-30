#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"

#include <QListWidgetItem>

DeviceInfoWidget::DeviceInfoWidget(QWidget *parent, Device *device,
                                   QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
  infoSections = std::vector<std::string>{"Device", "Network"};
}

DeviceInfoWidget::~DeviceInfoWidget() {}

/*
QWidget *DeviceInfoWidget::createWidget(std::string infoName) {
  QWidget *w = new QWidget(this->parentWidget());
  QPalette pal;
  pal.setColor(QPalette::Background, Qt::yellow);
  w->setPalette(pal);
  QGridLayout *lo = new QGridLayout();
  w->setLayout(lo);
  QLabel *l = new QLabel(w);
  l->setText(QString::fromStdString(infoName));
  lo->addWidget(l, 0, 0);
  return w;
}
*/
