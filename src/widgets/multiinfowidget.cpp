#include "multiinfowidget.h"
#include "ui_multiinfowidget.h"

MultiInfoWidget::MultiInfoWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), ui(new Ui::MultiInfoWidget), device(device) {
  ui->setupUi(this);
  QWidget *lTitleBar = titleBarWidget();
  setTitleBarWidget(new QWidget());
  delete lTitleBar;
  infoSections = {"Global", "Networking"};
  ui->infoList->addItem(tr("Global"));
  ui->infoList->addItem(tr("Networking"));
}

MultiInfoWidget::~MultiInfoWidget() { delete ui; }

void MultiInfoWidget::on_infoList_currentRowChanged(int currentRow) {
  std::string selectedInfo;
  QListWidgetItem *item = ui->infoList->item(currentRow);
  selectedInfo = item->text().toStdString();
  std::cout << "Current row " << currentRow << " Text: " << std::endl;
  QWidget *w = infoWidgets[selectedInfo];
  if (w == 0) {
    std::cout << "www" << currentRow << " Text: " << std::endl;
  }
  infoChanged(currentRow);
}
