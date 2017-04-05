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
  void readSettings();
  void writeSettings();

protected:
  void closeEvent(QCloseEvent *event);
};

#endif // MIOMAIN_H
