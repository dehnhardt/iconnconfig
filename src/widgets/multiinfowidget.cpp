#include "multiinfowidget.h"
#include "../miomain.h"
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
  std::string selectedInfo = infoSections[currentRow];
  QListWidgetItem *item = ui->infoList->item(currentRow);
  selectedInfo = item->text().toStdString();
  std::cout << "Current row " << currentRow << " Text: " << std::endl;
  QWidget *w = getWidget(selectedInfo);
  ((MioMain *)this->parentWidget())->setCentralWidget(w);
  infoChanged(currentRow);
}

QWidget *MultiInfoWidget::getWidget(std::string infoName) {
  QWidget *w = this->infoWidgets[infoName];
  if (w == 0) {
    w = createWidget(infoName);
    // this->infoWidgets.insert(std::pair<std::string, QWidget *>(infoName, w));
    this->infoWidgets[infoName] = w;
  }
  return w;
}
