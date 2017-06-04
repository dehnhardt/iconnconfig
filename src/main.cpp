#include "miomain.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("punkt-k");
  QCoreApplication::setOrganizationDomain("www.punkt-k.de");
  QCoreApplication::setApplicationName("Mio Config");

  /*QTranslator qtTranslator;
  qtTranslator.load("qt_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  app.installTranslator(&qtTranslator);*/

  QTranslator mioConfigTranslator;
  QString l = "mioconfig_" + QLocale::system().name();
  if (mioConfigTranslator.load(QLocale(), QLatin1String("mioconfig"),
                               QLatin1String("_"),
                               QLatin1String(":/translations/tr"))) {
    app.installTranslator(&mioConfigTranslator);
  }
  MioMain w;
  w.show();
  return app.exec();
}
