#ifndef MULTIINFOWIDGET_H
#define MULTIINFOWIDGET_H

#include "../device.h"
#include "../iconnconfigmain.h"
#include "multiinfolistentry.h"

#include <QDockWidget>
#include <QPalette>
#include <QShowEvent>
#include <QString>
#include <QWidget>

namespace Ui {
class MultiInfoWidget;
}

class MultiInfoWidget : public QDockWidget {

	Q_OBJECT

  public:
	explicit MultiInfoWidget(MioMain *parent = nullptr,
							 Device *device = nullptr,
							 QString windowTitle = tr("MultiInfoWidget"));
	~MultiInfoWidget();

	// members
  protected:
	Ui::MultiInfoWidget *ui;
	Device *device;
	int lastSelectedSection = 0;
	std::vector<MultiInfoListEntry *> *infoSections = nullptr;

  public:
	QMap<std::string, QWidget *> infoWidgets;
	void createInfoSections();
	int getFirstSelectableRow();

  protected:
	// methods
	QWidget *getWidget(int row);
	void openLastSelectedSection();
	virtual QWidget *createWidget(MultiInfoListEntry *entry) = 0;

  protected slots:
	void on_infoList_currentRowChanged(int currentRow);
	void visible(bool visible);

  signals:
	void infoTabChanged(int row);
};

#endif // MULTIINFOWIDGET_H
