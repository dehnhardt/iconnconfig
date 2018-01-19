#ifndef IPADDRESSINPUT_H
#define IPADDRESSINPUT_H

#include <QLineEdit>
#include <QObject>

class IPAddressInput : public QLineEdit {
	Q_OBJECT

public:
	IPAddressInput(QWidget *parent);
	bool getValid() const;

private:
	QString m_sIpRange;
	QRegExp m_IpRegex;
	bool m_bValid = false;

private slots:
	void validate(QString text);

};

#endif // IPADDRESSINPUT_H
