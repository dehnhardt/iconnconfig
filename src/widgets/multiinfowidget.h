#ifndef MULTIINFOWIDGET_H
#define MULTIINFOWIDGET_H

#include "../device.h"

#include <QDockWidget>
#include <QLabel>
#include <QLayout>
#include <QMap>
#include <QPalette>
#include <QWidget>

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

  // members
protected:
  Ui::MultiInfoWidget *ui;
  Device *device;

public:
  QMap<std::string, QWidget *> infoWidgets;
  std::vector<std::string> infoSections;

  // methods
protected:
  QWidget *getWidget(std::string infoName);
  virtual QWidget *createWidget(std::string infoName) {
    QWidget *w = new QWidget(this->parentWidget());
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::yellow);
    w->setPalette(pal);
    QGridLayout *lo = new QGridLayout();
    w->setLayout(lo);
    QLabel *l = new QLabel(w);
    l->setText(QString::fromStdString(infoName));
    lo->addWidget(l, 0, 0);
    return w;
  }

signals:
  void infoChanged(int row);
};

#endif // MULTIINFOWIDGET_H
