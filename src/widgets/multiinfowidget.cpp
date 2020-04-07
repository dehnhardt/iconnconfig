#include "multiinfowidget.h"
#include "../iconnconfigmain.h"
#include "ui_multiinfowidget.h"

MultiInfoWidget::MultiInfoWidget(MioMain *parent, Device *device,
								 QString windowTitle)
	: QDockWidget(parent), ui(new Ui::MultiInfoWidget), m_pDevice(device) {
	ui->setupUi(this);
	setWindowTitle(windowTitle);
	connect(this->ui->infoList, &QListWidget::currentRowChanged, this,
			&MultiInfoWidget::currentRowChanged);
	connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(visible(bool)));

	// hack to delete the titlebar
	QWidget *lTitleBar = titleBarWidget();
	setTitleBarWidget(new QWidget());
	delete lTitleBar;
}

MultiInfoWidget::~MultiInfoWidget() {
	disconnect(this->ui->infoList, &QListWidget::currentRowChanged, this,
			   &MultiInfoWidget::currentRowChanged);
	if (m_pInfoSections != nullptr) {
		std::vector<MultiInfoListEntry *>::iterator it;
		for (it = m_pInfoSections->begin(); it < m_pInfoSections->end(); ++it) {
			if (*it) {
				MultiInfoListEntry *e = (*it);
				delete e;
			}
		}
		m_pInfoSections->clear();
		delete m_pInfoSections;
	}
	delete ui;
}

void MultiInfoWidget::currentRowChanged(int currentRow) {
	MultiInfoListEntry *selectedInfo =
		m_pInfoSections->at(static_cast<unsigned long>(currentRow));
	if (!selectedInfo->enabled)
		return;
	if (selectedInfo->widget == nullptr) {
		selectedInfo->widget = createWidget(selectedInfo);
	}
	if (selectedInfo->widget) {
		QWidget *w = selectedInfo->widget;
		(static_cast<MioMain *>(this->parentWidget()))->replacePanel(w);
		emit infoTabChanged(currentRow);
	}
}

void MultiInfoWidget::visible(bool visible) {
	if (visible)
		openLastSelectedSection();
}

void MultiInfoWidget::createInfoSections() {
	std::vector<MultiInfoListEntry *>::iterator it;
	for (it = m_pInfoSections->begin(); it != m_pInfoSections->end(); ++it) {
		MultiInfoListEntry *entry = (*it);
		QListWidgetItem *item = new QListWidgetItem();
		entry->m_pListWidgetItem = item;
		if (entry->index == -1)
			item->setText(QString(entry->name.c_str()));
		else {
			QString qstr(entry->name.c_str());
			qstr.append(" ");
			QString num = QString::number(entry->index + 1);
			qstr.append(num);
			item->setText(qstr);
		}
		Qt::ItemFlags flags = item->flags();
		if (!entry->enabled)
			flags = flags & ~Qt::ItemIsEnabled;
		if (!entry->selectable)
			flags = flags & ~Qt::ItemIsSelectable;
		item->setFlags(flags);
		if (!entry->icon.isNull())
			item->setIcon(entry->icon);
		ui->infoList->addItem(item);
	}
}

int MultiInfoWidget::getFirstSelectableRow() {
	for (int i = 0; i < this->ui->infoList->count(); ++i) {
		MultiInfoListEntry *selectedInfo =
			m_pInfoSections->at(static_cast<unsigned long>(i));
		if (selectedInfo->enabled)
			return i;
	}
	return 0;
}

void MultiInfoWidget::openLastSelectedSection() {
	if (this->ui->infoList->currentRow() == -1) {
		this->ui->infoList->setCurrentRow(getFirstSelectableRow());
	} else {
		// on_infoList_currentRowChanged(this->ui->infoList->currentRow());
		currentRowChanged(this->ui->infoList->currentRow());
	}
}
