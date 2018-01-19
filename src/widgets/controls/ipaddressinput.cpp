#include "ipaddressinput.h"

IPAddressInput::IPAddressInput(QWidget *parent) : QLineEdit(parent) {
	m_sIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
	m_IpRegex = QRegExp("^" + m_sIpRange + "\\." + m_sIpRange + "\\." + m_sIpRange + "\\." +
										m_sIpRange + "$");
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
}

bool IPAddressInput::getValid() const { return m_bValid; }

void IPAddressInput::validate(QString text) {
	m_bValid = m_IpRegex.exactMatch(text);
	if (m_bValid)
		setStyleSheet("border: 1px solid green; border-radius: 2px; padding: 1px;");
	else
		setStyleSheet("border: 1px solid red; border-radius: 2px; padding: 1px;");
}
