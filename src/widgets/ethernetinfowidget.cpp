#include "ethernetinfowidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QTableWidget>

EthernetInfoWidget::EthernetInfoWidget(
    QWidget *parent, RetSetEthernetPortInfo *retSetEthernetPortInfo)
    : QWidget(parent), retSetEthernetPortInfo(retSetEthernetPortInfo) {

  lo = new QGridLayout();
	validateControls = new std::vector<IPAddressInput *>();
	updateTimer = new QTimer(this);
	updateTimer->setSingleShot(true);

  if (retSetEthernetPortInfo) {
    createWidgets();
    setupWidgets();
    setupLayout();
		createConnections();
		setData();
	}
  setLayout(lo);
}

EthernetInfoWidget::~EthernetInfoWidget() {

  delete methodBox;

  delete tl;
  delete ipl1;
  delete sml1;
  delete gwl1;
  delete ipl2;
  delete sml2;
  delete gwl2;
  delete bjl;
  delete macl;

  delete ip1;
  delete sm1;
  delete gw1;
  delete ip2;
  delete sm2;
  delete gw2;
  delete bj;
  delete mac;

  delete empty;

  delete lo;
  delete staticLayout;
  delete dhcpLayout;
  delete infoLayout;

  delete staticBox;
  delete dhcpBox;
  delete infoBox;
}

void EthernetInfoWidget::createWidgets() {

	iPAddressInputSignalMapper = new QSignalMapper();

  lo = new QGridLayout();
  staticLayout = new QGridLayout();
  dhcpLayout = new QGridLayout();
  infoLayout = new QGridLayout();

  methodBox = new QComboBox(this);

  staticBox = new QGroupBox(tr("Static Address"), this);
  dhcpBox = new QGroupBox(tr("DHCP Address"), this);
  infoBox = new QGroupBox(tr("Info"), this);

  tl = new QLabel(tr("Address Type"));
  ipl1 = new QLabel(tr("Static IP"), staticBox);
  sml1 = new QLabel(tr("Subnet Mask"), staticBox);
  gwl1 = new QLabel(tr("Gateway"), staticBox);
  ipl2 = new QLabel(tr("DHCP IP"), this);
  sml2 = new QLabel(tr("Subnet Mask"), this);
  gwl2 = new QLabel(tr("Gateway"), this);
  bjl = new QLabel(tr("Bonjour Name"), infoBox);
  macl = new QLabel(tr("MAC Address"), infoBox);

	ip1 = new IPAddressInput(staticBox);
	sm1 = new IPAddressInput(staticBox);
	gw1 = new IPAddressInput(staticBox);
  ip2 = new QLineEdit(dhcpBox);
  sm2 = new QLineEdit(dhcpBox);
  gw2 = new QLineEdit(dhcpBox);
  bj = new QLineEdit(infoBox);
  mac = new QLineEdit(infoBox);

  empty = new QWidget();
}

void EthernetInfoWidget::setupWidgets() {

  QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegExp ipRegex("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." +
                  ipRange + "$");

	validateControls->push_back(ip1);
	validateControls->push_back(sm1);
	validateControls->push_back(gw1);

  mac->setInputMask("nn:nn:nn:nn:nn:nn;_");

  dhcpBox->setDisabled(true);
  infoBox->setDisabled(true);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void EthernetInfoWidget::setupLayout() {

  lo->addWidget(tl, 0, 0);
  lo->addWidget(methodBox, 0, 1);

  staticLayout->addWidget(ipl1, 0, 0);
  staticLayout->addWidget(ip1, 0, 1);

  staticLayout->addWidget(sml1, 1, 0);
  staticLayout->addWidget(sm1, 1, 1);

  staticLayout->addWidget(gwl1, 2, 0);
  staticLayout->addWidget(gw1, 2, 1);

  dhcpLayout->addWidget(ipl2, 0, 0);
  dhcpLayout->addWidget(ip2, 0, 1);

  dhcpLayout->addWidget(sml2, 1, 0);
  dhcpLayout->addWidget(sm2, 1, 1);

  dhcpLayout->addWidget(gwl2, 2, 0);
  dhcpLayout->addWidget(gw2, 2, 1);

  infoLayout->addWidget(bjl, 0, 0);
  infoLayout->addWidget(bj, 0, 1);
  infoLayout->addWidget(macl, 1, 0);
  infoLayout->addWidget(mac, 1, 1);

  staticBox->setLayout(staticLayout);
  dhcpBox->setLayout(dhcpLayout);
  infoBox->setLayout(infoLayout);

  lo->addWidget(staticBox, 1, 0, 1, 2);
  lo->addWidget(dhcpBox, 2, 0, 1, 2);
  lo->addWidget(infoBox, 3, 0, 1, 2);
  lo->addWidget(empty, 3, 0, 1, 2);
}

void EthernetInfoWidget::setData() {
  methodBox->addItem(tr("Static"),
                     QVariant((int)RetSetEthernetPortInfo::STATIC));
  methodBox->addItem(tr("Dynamic"),
                     QVariant((int)RetSetEthernetPortInfo::DYNAMIC));

  ip1->setText(QString(retSetEthernetPortInfo
                           ->getAddress(RetSetEthernetPortInfo::STATIC |
                                        RetSetEthernetPortInfo::ADDRESS)
                           .c_str()));
  sm1->setText(QString(retSetEthernetPortInfo
                           ->getAddress(RetSetEthernetPortInfo::STATIC |
                                        RetSetEthernetPortInfo::SUBNET_MASK)
                           .c_str()));
  gw1->setText(QString(retSetEthernetPortInfo
                           ->getAddress(RetSetEthernetPortInfo::STATIC |
                                        RetSetEthernetPortInfo::GATEWAY)
                           .c_str()));
  ip2->setText(QString(retSetEthernetPortInfo
                           ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
                                        RetSetEthernetPortInfo::ADDRESS)
                           .c_str()));
  sm2->setText(QString(retSetEthernetPortInfo
                           ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
                                        RetSetEthernetPortInfo::SUBNET_MASK)
                           .c_str()));
  gw2->setText(QString(retSetEthernetPortInfo
                           ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
                                        RetSetEthernetPortInfo::GATEWAY)
                           .c_str()));
  bj->setText(QString(retSetEthernetPortInfo->getBonjourName().c_str()));
  mac->setText(QString(retSetEthernetPortInfo->getMacAddress().c_str()));

  int index = methodBox->findData((int)retSetEthernetPortInfo->getMethod());
  if (index != -1) { // -1 for not found
    methodBox->setCurrentIndex(index);
    setStaticBoxEnabled(index);
  }
}

void EthernetInfoWidget::createConnections() {
	connect(iPAddressInputSignalMapper, SIGNAL(mapped(QObject *)), this,
					SLOT(editFinished(QObject *)));

  connect(methodBox, SIGNAL(activated(int)), this, SLOT(comboboxSelected(int)));
  connect(this, SIGNAL(staticBoxDisabled(bool)), staticBox,
          SLOT(setDisabled(bool)));

	connect(ip1, SIGNAL(editingFinished()), iPAddressInputSignalMapper,
					SLOT(map()));
	iPAddressInputSignalMapper->setMapping(
			ip1, new IPAddressInputMapper(ip1, RetSetEthernetPortInfo::STATIC |
																						 RetSetEthernetPortInfo::ADDRESS));
	connect(sm1, SIGNAL(editingFinished()), iPAddressInputSignalMapper,
					SLOT(map()));
	iPAddressInputSignalMapper->setMapping(
			sm1,
			new IPAddressInputMapper(sm1, RetSetEthernetPortInfo::STATIC |
																				RetSetEthernetPortInfo::SUBNET_MASK));
	connect(gw1, SIGNAL(editingFinished()), iPAddressInputSignalMapper,
					SLOT(map()));
	iPAddressInputSignalMapper->setMapping(
			ip1, new IPAddressInputMapper(gw1, RetSetEthernetPortInfo::STATIC |
																						 RetSetEthernetPortInfo::GATEWAY));

  connect(this, SIGNAL(staticBoxDisabled(bool)), staticBox,
          SLOT(setDisabled(bool)));

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateEthernetConfig()));
}

void EthernetInfoWidget::setStaticBoxEnabled(int selected) {
  bool disabled = (selected == 1);
	emit staticBoxDisabled(disabled);
}

bool EthernetInfoWidget::ipAddressControlsValid() {
	std::vector<IPAddressInput *>::iterator it;
	for (it = validateControls->begin(); it != validateControls->end(); ++it) {
		IPAddressInput *input = (*it);
		if (!input->getValid())
			return false;
	}
	return true;
}

void EthernetInfoWidget::comboboxSelected(int selected) {
  setStaticBoxEnabled(selected);
	retSetEthernetPortInfo->setMethod(
			(RetSetEthernetPortInfo::IPFlags)methodBox->currentData().Int);
	updateTimer->start(1000);
}

void EthernetInfoWidget::editFinished(QObject *object) {
	IPAddressInputMapper *m = (IPAddressInputMapper *)object;
	IPAddressInput *input = m->input;
	std::cout << "Edit finished" << std::endl;
	if (m->input->getValid()) {
		retSetEthernetPortInfo->setAddress(m->flags,
																			 m->input->text().toStdString());
		updateTimer->start(1000);
	}
}

void EthernetInfoWidget::updateEthernetConfig() {
	if (ipAddressControlsValid()) {
		std::cout << "all controls valid" << std::endl;
	} else {
		std::cout << "at least one control invalid" << std::endl;
	}
}
