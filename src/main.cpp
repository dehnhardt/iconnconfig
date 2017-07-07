#include "miomain.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("punkt-k");
  QCoreApplication::setOrganizationDomain("www.punkt-k.de");
  QCoreApplication::setApplicationName("Mio Config");

  QTranslator mioConfigTranslator;
  QString l = "mioconfig_" + QLocale::system().name();
  if (mioConfigTranslator.load(QLocale(), QLatin1String("mioconfig"),
                               QLatin1String("_"),
                               QLatin1String(":/translations/tr"))) {
    app.installTranslator(&mioConfigTranslator);
  }

	QCommandLineParser parser;
	parser.setApplicationDescription("Mio Config");
	parser.addHelpOption();
	parser.addVersionOption();

	// A boolean option with multiple names (-f, --filename)
	QCommandLineOption filenameOption(
			QStringList() << "f"
										<< "filename",
			QCoreApplication::translate("main", "filename to read settings from."),
			"filename");
	parser.addOption(filenameOption);

	// Process the actual command line arguments given by the user
	parser.process(app);
  MioMain w;
	if (parser.isSet("filename")) {
		QString fileName = parser.value("filename");
		QFile *configurationFile = new QFile(fileName);
		w.setConfigurationFile(configurationFile);
	}
	w.show();
  return app.exec();
}
