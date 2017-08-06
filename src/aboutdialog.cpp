#include "aboutdialog.h"
#include "config/configuration.h"
#include "ui_aboutdialog.h"

#include <QFrame>
#include <QLabel>

AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent), ui(new Ui::AboutDialog) {
	ui->setupUi(this);
	setupScrollArea();
	setupTable();
}

AboutDialog::~AboutDialog() { delete ui; }

void AboutDialog::setupScrollArea() {
	layout = new QGridLayout();
	ui->scrollAreaWidgetContents->setLayout(layout);
}

void AboutDialog::addEntry(const QString label, const QString value,
						   bool header) {
	int colspan = 1;
	if (header) {
		colspan = 2;
		QFrame *line = new QFrame(this);
		line->setObjectName(QStringLiteral("line"));
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		layout->addWidget(line, row, 0, 1, colspan);
		++row;
	}
	layout->addWidget(getLabel(label, header), row, 0, 1, colspan);
	if (!header)
		layout->addWidget(getLabel(value), row, 1);
	++row;
}

void AboutDialog::setupTable() {
	addEntry(QApplication::applicationName(), NULL, true);
	// Name
	addEntry(tr("Application Name"), QApplication::applicationName());
	// Version
	addEntry(tr("Version"), QApplication::applicationVersion());
	// author
	addEntry(tr("Author"), AUTHOR);
	// organization
	addEntry(tr("Organization"), QApplication::organizationName());
	// License
	addEntry(tr("License"), LICENSE);
	// Used Software
	addEntry(tr("Used Software"), NULL, true);
	// RtMIDI
	addEntry(tr("RtMIDI"),
			 tr("All midi in- and output is realized with the RtMidi "
				"Library. Created at <a "
				"href='https://ccrma.stanford.edu/software/stk/"
				"classRtMidi.html'>Stanford University</a> "
				"<br/>Copyright ©1995--2016 Perry R. Cook and Gary P. Scavone. "
				"All Rights Reserved."));
	// Icons
	addEntry(tr("Used Icons"), NULL, true);
	// USB Host
	addEntry(tr("USB Device Jack Icon"), tr("Downloaded from the <a "
											"href='https://thenounproject.com/"
											"'>NOUN Project</a><br/>"
											"\nCreated by Okan Benn"));
	// Ethernet Jack
	addEntry(tr("Ethernet Jack Icon"), tr("Downloaded from the <a "
										  "href='https://thenounproject.com/"
										  "'>NOUN Project</a><br/>"
										  "\nCreated by Michael Wohlwend"));
	addEntry(tr("Credits"), NULL, true);
	// Ralf Beck - MTP-Driver
	addEntry(tr("RTP-Driver"),
			 tr("Ralf Beck created an RTP-Driver which I use "
				"often to connect to the MIO10 when doing "
				"boring programming routines in front of an exciting "
				"film."
				"\nAs well he provided a patch to make MioConfig "
				"work with QT-Version 4.8."));
}

QLabel *AboutDialog::getLabel(QLabel *label, const bool header) {
	if (header) {
		QFont f = label->font();
		f.setBold(true);
		f.setPointSize(f.pointSize() + 2);
		label->setFont(f);
	}
	label->setWordWrap(true);
	label->setAlignment(label->alignment() | Qt::AlignmentFlag::AlignTop);
	return label;
}

QLabel *AboutDialog::getLabel(const char *text, const bool header) {
	QLabel *l = new QLabel(text);
	return getLabel(l, header);
}

QLabel *AboutDialog::getLabel(const QString text, const bool header) {
	QLabel *l = new QLabel(text);
	return getLabel(l, header);
}
