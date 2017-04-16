#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"

#include <QListWidgetItem>

DeviceInfoWidget::DeviceInfoWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), ui(new Ui::DeviceInfoWidget), device(device) {
  ui->setupUi(this);
  QWidget *lTitleBar = titleBarWidget();
  setTitleBarWidget(new QWidget());
  delete lTitleBar;
  ui->infoList->addItem(tr("Global"));
  ui->infoList->addItem(tr("Networking"));
}

DeviceInfoWidget::~DeviceInfoWidget() { delete ui; }

void DeviceInfoWidget::on_infoList_currentRowChanged(int currentRow) {
  QListWidgetItem *item = ui->infoList->item(currentRow);
  std::cout << "Current row " << currentRow
            << " Text: " << item->text().toStdString() << std::endl;
  infoChanged(currentRow);
}
