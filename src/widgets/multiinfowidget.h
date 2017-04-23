#ifndef MULTIINFOWIDGET_H
#define MULTIINFOWIDGET_H

#include "../device.h"

#include <QDockWidget>

namespace Ui {
class MultiInfoWidget;
}

class MultiInfoWidget : public QDockWidget {
  Q_OBJECT

public:
  explicit MultiInfoWidget(QWidget *parent = 0, Device *device = 0);
  ~MultiInfoWidget();

protected slots:
  void on_infoList_currentRowChanged(int currentRow);

protected:
  Ui::MultiInfoWidget *ui;
  Device *device;
  std::map<std::string, QWidget *> infoWidgets;
  std::vector<std::string> infoSections;

signals:
  void infoChanged(int row);
};

#endif // MULTIINFOWIDGET_H
