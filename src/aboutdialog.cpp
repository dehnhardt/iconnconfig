#include "aboutdialog.h"
#include "config/configuration.h"
#include "ui_aboutdialog.h"

#include <QBitmap>
#include <QFrame>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QPicture>

AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent), m_pUi(new Ui::AboutDialog) {
	m_pUi->setupUi(this);
	setupScrollArea();
	setupTable();
}

AboutDialog::~AboutDialog() { delete m_pUi; }

void AboutDialog::setupScrollArea() {
	m_pLlayout = new QGridLayout();
	m_pUi->scrollAreaWidgetContents->setLayout(m_pLlayout);
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
		m_pLlayout->addWidget(line, m_iRow, 0, 1, colspan);
		++m_iRow;
	}
	m_pLlayout->addWidget(getLabel(label, header), m_iRow, 0, 1, colspan);
	if (!header)
		m_pLlayout->addWidget(getLabel(value), m_iRow, 1);
	++m_iRow;
}

void AboutDialog::addEntry(QLabel *label, const QString value) {
	m_pLlayout->addWidget(label, m_iRow, 0);
	m_pLlayout->addWidget(getLabel(value), m_iRow, 1);
	++m_iRow;
}

void AboutDialog::setupTable() {
	QIcon icon(":/pixmaps/appicon");
	QLabel *iconLabel = new QLabel;
	iconLabel->setPixmap(icon.pixmap(60, 60));
	addEntry(iconLabel, nullptr);
	addEntry(QApplication::applicationName(), nullptr, true);
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
	addEntry(tr("Used Software"), nullptr, true);
	// RtMIDI
	addEntry(tr("RtMIDI"),
			 tr("All midi in- and output is realized with the RtMidi "
				"Library. Created at <a "
				"href='https://ccrma.stanford.edu/software/stk/"
				"classRtMidi.html'>Stanford University</a> "
				"<br/>Copyright ©1995--2016 Perry R. Cook and Gary P. Scavone. "
				"All Rights Reserved."));
	// Icons
	addEntry(tr("Used Icons"), nullptr, true);
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
	// XLR Jack
	addEntry(tr("XLR Jack Icon"), tr("Downloaded from the <a "
									 "href='https://thenounproject.com/"
									 "'>NOUN Project</a><br/>"
									 "\nCreated by Chad Wimberly"));
	addEntry(tr("Credits"), nullptr, true);
	// Ralf Beck - MTP-Driver
	addEntry(tr("RTP-Driver"),
			 tr("Ralf Beck created an RTP-Driver which I use "
				"often to connect to the MIO10 when doing "
				"boring programming routines in front of an exciting "
				"film."
				"\nAs well he provided a patch to make MioConfig "
				"work with QT-Version 4.8."));
	addEntry(tr("Translations"), tr("French: ") + "Olivier Humbert");
	addEntry(tr("General help from"), "JP Cimalando");
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
