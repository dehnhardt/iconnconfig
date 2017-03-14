#include "miomain.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MioMain w;
  w.show();
  return a.exec();
}
