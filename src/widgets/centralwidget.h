#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "../device.h"

#include <QDockWidget>
#include <QGridLayout>
#include <QObject>
#include <QWidget>

class CentralWidget : public QDockWidget {
  Q_OBJECT

public:
  CentralWidget(QWidget *parent, Device *m_pDevice);
  void replacePanel(QWidget *w);

private:
  QWidget *m_pCurrentWidget = 0;
  Device *m_pDevice;
  QGridLayout *m_pLayout = 0;
};

#endif // CENTRALWIDGET_H
