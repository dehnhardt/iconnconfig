#ifndef MIOMAIN_H
#define MIOMAIN_H

#include "devicedetection.h"
#include <QMainWindow>

namespace Ui {
class MioMain;
}

class MioMain : public QMainWindow {
  Q_OBJECT

public:
  explicit MioMain(QWidget *parent = 0);
  ~MioMain();

private slots:
  void on_actionQuit_triggered();
  void openDetectionWindow();

private:
  // Members
  Ui::MioMain *ui;
  DeviceDetection *deviceDetectionWindow;
};

#endif // MIOMAIN_H
