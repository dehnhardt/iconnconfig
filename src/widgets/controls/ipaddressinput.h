#ifndef IPADDRESSINPUT_H
#define IPADDRESSINPUT_H

#include <QLineEdit>
#include <QObject>

class IPAddressInput : public QLineEdit {
	Q_OBJECT

public:
	IPAddressInput(QWidget *parent);

	bool getValid() const;

private slots:
	void validate(QString text);

private:
	QString ipRange;
	QRegExp ipRegex;
	bool valid = false;
};

#endif // IPADDRESSINPUT_H
