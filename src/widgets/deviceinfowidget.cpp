#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"

#include <QListWidgetItem>

DeviceInfoWidget::DeviceInfoWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), MultiInfo(parent), ui(new Ui::DeviceInfoWidget),
      device(device) {
  ui->setupUi(this);
  QWidget *lTitleBar = titleBarWidget();
  setTitleBarWidget(new QWidget());
  delete lTitleBar;
  infoSections = {"Global", "Networking"};
  ui->infoList->addItem(tr("Global"));
  ui->infoList->addItem(tr("Networking"));
}

DeviceInfoWidget::~DeviceInfoWidget() { delete ui; }

void DeviceInfoWidget::on_infoList_currentRowChanged(int currentRow) {
  std::string selectedInfo;
  QListWidgetItem *item = ui->infoList->item(currentRow);
  selectedInfo = item->text().toStdString();
  std::cout << "Current row " << currentRow << " Text: " << std::endl;
  QWidget *w = mainWidgets[selectedInfo];
  if (w == 0) {
  }
  infoChanged(currentRow);
}
