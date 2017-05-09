#ifndef MULTIINFOWIDGET_H
#define MULTIINFOWIDGET_H

#include "../device.h"
#include "../miomain.h"

#include <QDockWidget>
#include <QLabel>
#include <QLayout>
#include <QMap>
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
	explicit MultiInfoWidget(MioMain *parent = 0, Device *device = 0,
                           QString windowTitle = tr("MultiInfoWidget"));
  ~MultiInfoWidget();

protected slots:
  void on_infoList_currentRowChanged(int currentRow);
  void visible(bool visible);

  // members
protected:
  Ui::MultiInfoWidget *ui;
  Device *device;
  std::vector<std::string> infoSections = {"Info 1", "Info 2"};
  int lastSelectedSection = 0;

public:
  QMap<std::string, QWidget *> infoWidgets;
  void createInfoSections();

  // methods
protected:
  QWidget *getWidget(std::string infoName);
  void openLastSelectedSection();
	virtual QWidget *createWidget(std::string infoName) = 0;

signals:
  void infoChanged(int row);
};

#endif // MULTIINFOWIDGET_H
