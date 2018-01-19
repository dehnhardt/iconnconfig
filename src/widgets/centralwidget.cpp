#include "centralwidget.h"

#include <QLayoutItem>

CentralWidget::CentralWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), m_pDevice(device) {
  setFeatures(NoDockWidgetFeatures);
  setAllowedAreas(Qt::NoDockWidgetArea);
  QWidget *w = new QWidget(this);
  setWidget(w);
  m_pLayout = new QGridLayout();
  w->setLayout(m_pLayout);
}

void CentralWidget::replacePanel(QWidget *w) { setWidget(w); }
