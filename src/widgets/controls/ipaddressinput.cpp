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
		setStyleSheet("QLineEdit { background: rgb(0, 255, 255); "
									"selection-background-color: rgb(233, 99, 0); }");
	else
		setStyleSheet("QLineEdit { background: rgb(255, 0, 255); "
									"selection-background-color: rgb(233, 0, 99); }");
}
