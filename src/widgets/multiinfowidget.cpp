#include "multiinfowidget.h"
#include "../miomain.h"
#include "ui_multiinfowidget.h"

MultiInfoWidget::MultiInfoWidget(MioMain *parent, Device *device,
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
	MultiInfoListEntry selectedInfo = (*infoSections)[currentRow];
	if (selectedInfo.widget == 0) {
		selectedInfo.widget = createWidget(selectedInfo);
	}
	if (selectedInfo.widget) {
		QWidget *w = selectedInfo.widget;
		((MioMain *)this->parentWidget())->replacePanel(w);
		emit infoTabChanged(currentRow);
	}
}

void MultiInfoWidget::visible(bool visible) {
  if (visible)
    openLastSelectedSection();
}

void MultiInfoWidget::createInfoSections() {
	std::vector<MultiInfoListEntry>::iterator it;
	for (it = infoSections->begin(); it < infoSections->end(); ++it) {
		MultiInfoListEntry entry = *it;
		QString item;
		if (entry.index == -1)
			item = QString(entry.name.c_str());
		else
			item = QString::asprintf("%s %i", entry.name.c_str(), entry.index);
		ui->infoList->addItem(item);
  }
}

void MultiInfoWidget::openLastSelectedSection() {
  if (this->ui->infoList->currentRow() == -1) {
    this->ui->infoList->setCurrentRow(0);
  } else {
    on_infoList_currentRowChanged(this->ui->infoList->currentRow());
  }
}
