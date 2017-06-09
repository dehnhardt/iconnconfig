#include "ipaddressinput.h"

IPAddressInput::IPAddressInput(QWidget *parent) : QLineEdit(parent) {
	ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
	ipRegex = QRegExp("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." +
										ipRange + "$");
	//*regValidator = new QRegExpValidator(ipRegex, 0);
	// setValidator(regValidator);
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
}

bool IPAddressInput::getValid() const { return valid; }

void IPAddressInput::validate(QString text) {
	valid = ipRegex.exactMatch(text);
	if (valid)
		setStyleSheet("border: 1px solid green; border-radius: 2px; padding: 1px;");
	else
		setStyleSheet("border: 1px solid red; border-radius: 2px; padding: 1px;");
}
