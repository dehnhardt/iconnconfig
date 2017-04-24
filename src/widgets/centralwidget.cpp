#include "centralwidget.h"

#include <QLayoutItem>

CentralWidget::CentralWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), device(device) {
  setFeatures(NoDockWidgetFeatures);
  setAllowedAreas(Qt::NoDockWidgetArea);
  QWidget *w = new QWidget(this);
  setWidget(w);
  l = new QGridLayout();
  w->setLayout(l);
}

void CentralWidget::replacePanel(QWidget *w) { setWidget(w); }
