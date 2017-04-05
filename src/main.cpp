#include "miomain.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("punkt-k");
  QCoreApplication::setOrganizationDomain("www.punkt-k.de");
  QCoreApplication::setApplicationName("Mio Config");
  MioMain w;
  w.show();
  return a.exec();
}
