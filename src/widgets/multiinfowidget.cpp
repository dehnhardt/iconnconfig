#include "multiinfowidget.h"
#include "../miomain.h"
#include "ui_multiinfowidget.h"

MultiInfoWidget::MultiInfoWidget(QWidget *parent, Device *device,
                                 QString windowTitle)
    : QDockWidget(parent), ui(new Ui::MultiInfoWidget), device(device) {
  ui->setupUi(this);
  setWindowTitle(windowTitle);
  connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(visible(bool)));
  // hack to delete the titlebar
  QWidget *lTitleBar = titleBarWidget();
  setTitleBarWidget(new QWidget());
  delete lTitleBar;
}

MultiInfoWidget::~MultiInfoWidget() { delete ui; }

void MultiInfoWidget::on_infoList_currentRowChanged(int currentRow) {
  std::string selectedInfo = infoSections[currentRow];
  QListWidgetItem *item = ui->infoList->item(currentRow);
  selectedInfo = item->text().toStdString();
  std::cout << "Current row " << currentRow << " Text: " << std::endl;
  QWidget *w = getWidget(selectedInfo);
  ((MioMain *)this->parentWidget())->replacePanel(w);
  infoChanged(currentRow);
}

void MultiInfoWidget::visible(bool visible) {
  if (visible)
    openLastSelectedSection();
}

void MultiInfoWidget::createInfoSections() {
  std::vector<std::string>::iterator it;
  for (it = infoSections.begin(); it < infoSections.end(); ++it) {
    std::string v = *it;
    ui->infoList->addItem(tr(v.c_str()));
  }
}

QWidget *MultiInfoWidget::getWidget(std::string infoName) {
  QWidget *w = this->infoWidgets[infoName];
  if (w == 0) {
    w = createWidget(infoName);
    this->infoWidgets[infoName] = w;
  }
  return w;
}

void MultiInfoWidget::openLastSelectedSection() {
  if (this->ui->infoList->currentRow() == -1) {
    this->ui->infoList->setCurrentRow(0);
  } else {
    on_infoList_currentRowChanged(this->ui->infoList->currentRow());
  }
}
