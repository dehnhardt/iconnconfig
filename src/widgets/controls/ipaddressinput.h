#ifndef IPADDRESSINPUT_H
#define IPADDRESSINPUT_H

#include <QLineEdit>
#include <QObject>

class IPAddressInput : public QLineEdit {
public:
	IPAddressInput(QWidget *parent);

	bool getValid() const;

private slots:
	void validate(QString text);

private:
	QString ipRange;
	QRegExp ipRegex;
	// QRegExpValidator *regValidator = 0;
	bool valid = false;
};

#endif // IPADDRESSINPUT_H
