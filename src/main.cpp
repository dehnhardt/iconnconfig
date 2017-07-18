#include "miomain.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QLibraryInfo>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("punkt-k");
	QCoreApplication::setOrganizationDomain("www.punkt-k.de");
	QCoreApplication::setApplicationName("Mio Config");
	QCoreApplication::setApplicationVersion("0.1.5-alpha");

	QTranslator mioConfigTranslator;
	QString l = "mioconfig_" + QLocale::system().name();
	if (mioConfigTranslator.load(QLocale(), QLatin1String("mioconfig"),
								 QLatin1String("_"),
								 QLatin1String(":/translations/tr"))) {
		app.installTranslator(&mioConfigTranslator);
	}

	QCommandLineParser *parser = new QCommandLineParser();
	parser->setApplicationDescription("Mio Config");
	parser->addHelpOption();
	parser->addVersionOption();

	// A boolean option with multiple names (-f, --filename)
	QCommandLineOption filenameOption(
		QStringList() << "f"
					  << "filename",
		QCoreApplication::translate("main", "filename to read settings from."),
		"filename");
	parser->addOption(filenameOption);

	// Process the actual command line arguments given by the user
	parser->process(app);
	MioMain w(parser);
	w.show();
	return app.exec();
}
