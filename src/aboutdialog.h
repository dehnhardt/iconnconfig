#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>

namespace Ui
{
class AboutDialog;
}

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDialog(QWidget *parent = 0);
	~AboutDialog();

private:
	Ui::AboutDialog *m_pUi;
	QGridLayout *m_pLlayout = 0;
	int m_iRow = 0;

	void setupTable();
	QLabel *getLabel(const char *text, const bool header = false);
	QLabel *getLabel(const QString text, const bool header = false);
	QLabel *getLabel(QLabel *label, const bool header);
	void addEntry(const QString label, const QString value,
				  bool header = false);
	void setupScrollArea();
};

#endif// ABOUTDIALOG_H
