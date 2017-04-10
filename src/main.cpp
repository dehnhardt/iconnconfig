#include "miomain.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("punkt-k");
  QCoreApplication::setOrganizationDomain("www.punkt-k.de");
  QCoreApplication::setApplicationName("Mio Config");

  QTranslator qtTranslator;
  qtTranslator.load("qt_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  app.installTranslator(&qtTranslator);

  QTranslator myappTranslator;
  myappTranslator.load("myapp_" + QLocale::system().name());
  app.installTranslator(&myappTranslator);

  MioMain w;
  w.show();
  return app.exec();
}
