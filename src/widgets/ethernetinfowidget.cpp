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
	: QWidget(parent), m_pRetSetEthernetPortInfo(retSetEthernetPortInfo) {

	m_pRootLayout = new QGridLayout();
	m_pValidateControls = new std::vector<IPAddressInput *>();
	r_mUpdateTimer = new QTimer(this);
	r_mUpdateTimer->setSingleShot(true);

	if (retSetEthernetPortInfo) {
		createWidgets();
		setupWidgets();
		setupLayout();
		createConnections();
		setData();
	}
	setLayout(m_pRootLayout);
}

EthernetInfoWidget::~EthernetInfoWidget() {

	delete m_pMethodBox;

	delete m_pTl;
	delete m_pIpl1;
	delete m_pSml1;
	delete m_pGwl1;
	delete m_PIpl2;
	delete m_pSml2;
	delete m_pGwl2;
	delete m_pBjl;
	delete m_pMacl;

	delete m_Ip1;
	delete m_pSm1;
	delete m_pGw1;
	delete m_pIp2;
	delete m_pSm2;
	delete m_pGw2;
	delete m_pBj;
	delete m_pMac;

	delete m_pEmpty;

	delete m_pRootLayout;
	delete m_pStaticLayout;
	delete m_pDhcpLayout;
	delete m_pInfoLayout;

	delete m_pStaticBox;
	delete m_pDhcpBox;
	delete m_pInfoBox;
}

void EthernetInfoWidget::createWidgets() {

	iPAddressInputSignalMapper = new QSignalMapper();

	m_pRootLayout = new QGridLayout();
	m_pStaticLayout = new QGridLayout();
	m_pDhcpLayout = new QGridLayout();
	m_pInfoLayout = new QGridLayout();

	m_pMethodBox = new QComboBox(this);

	m_pStaticBox = new QGroupBox(tr("Static Address"), this);
	m_pDhcpBox = new QGroupBox(tr("DHCP Address"), this);
	m_pInfoBox = new QGroupBox(tr("Info"), this);

	m_pTl = new QLabel(tr("Address Type"));
	m_pIpl1 = new QLabel(tr("Static IP"), m_pStaticBox);
	m_pSml1 = new QLabel(tr("Subnet Mask"), m_pStaticBox);
	m_pGwl1 = new QLabel(tr("Gateway"), m_pStaticBox);
	m_PIpl2 = new QLabel(tr("DHCP IP"), this);
	m_pSml2 = new QLabel(tr("Subnet Mask"), this);
	m_pGwl2 = new QLabel(tr("Gateway"), this);
	m_pBjl = new QLabel(tr("Bonjour Name"), m_pInfoBox);
	m_pMacl = new QLabel(tr("MAC Address"), m_pInfoBox);

	m_Ip1 = new IPAddressInput(m_pStaticBox);
	m_pSm1 = new IPAddressInput(m_pStaticBox);
	m_pGw1 = new IPAddressInput(m_pStaticBox);
	m_pIp2 = new QLineEdit(m_pDhcpBox);
	m_pSm2 = new QLineEdit(m_pDhcpBox);
	m_pGw2 = new QLineEdit(m_pDhcpBox);
	m_pBj = new QLineEdit(m_pInfoBox);
	m_pMac = new QLineEdit(m_pInfoBox);

	m_pEmpty = new QWidget();
}

void EthernetInfoWidget::setupWidgets() {

	QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
	QRegExp ipRegex("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." +
					ipRange + "$");

	m_pValidateControls->push_back(m_Ip1);
	m_pValidateControls->push_back(m_pSm1);
	m_pValidateControls->push_back(m_pGw1);

	m_pMac->setInputMask("nn:nn:nn:nn:nn:nn;_");

	m_pDhcpBox->setDisabled(true);
	m_pInfoBox->setDisabled(true);
	m_pEmpty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void EthernetInfoWidget::setupLayout() {

	m_pRootLayout->addWidget(m_pTl, 0, 0);
	m_pRootLayout->addWidget(m_pMethodBox, 0, 1);

	m_pStaticLayout->addWidget(m_pIpl1, 0, 0);
	m_pStaticLayout->addWidget(m_Ip1, 0, 1);

	m_pStaticLayout->addWidget(m_pSml1, 1, 0);
	m_pStaticLayout->addWidget(m_pSm1, 1, 1);

	m_pStaticLayout->addWidget(m_pGwl1, 2, 0);
	m_pStaticLayout->addWidget(m_pGw1, 2, 1);

	m_pDhcpLayout->addWidget(m_PIpl2, 0, 0);
	m_pDhcpLayout->addWidget(m_pIp2, 0, 1);

	m_pDhcpLayout->addWidget(m_pSml2, 1, 0);
	m_pDhcpLayout->addWidget(m_pSm2, 1, 1);

	m_pDhcpLayout->addWidget(m_pGwl2, 2, 0);
	m_pDhcpLayout->addWidget(m_pGw2, 2, 1);

	m_pInfoLayout->addWidget(m_pBjl, 0, 0);
	m_pInfoLayout->addWidget(m_pBj, 0, 1);
	m_pInfoLayout->addWidget(m_pMacl, 1, 0);
	m_pInfoLayout->addWidget(m_pMac, 1, 1);

	m_pStaticBox->setLayout(m_pStaticLayout);
	m_pDhcpBox->setLayout(m_pDhcpLayout);
	m_pInfoBox->setLayout(m_pInfoLayout);

	m_pRootLayout->addWidget(m_pStaticBox, 1, 0, 1, 2);
	m_pRootLayout->addWidget(m_pDhcpBox, 2, 0, 1, 2);
	m_pRootLayout->addWidget(m_pInfoBox, 3, 0, 1, 2);
	m_pRootLayout->addWidget(m_pEmpty, 3, 0, 1, 2);
}

void EthernetInfoWidget::setData() {
	m_pMethodBox->addItem(tr("Static"), QVariant(static_cast<int>(
										 RetSetEthernetPortInfo::STATIC)));
	m_pMethodBox->addItem(tr("Dynamic"), QVariant(static_cast<int>(
										  RetSetEthernetPortInfo::DYNAMIC)));

	m_Ip1->setText(QString(m_pRetSetEthernetPortInfo
							 ->getAddress(RetSetEthernetPortInfo::STATIC |
										  RetSetEthernetPortInfo::ADDRESS)
							 .c_str()));
	m_pSm1->setText(QString(m_pRetSetEthernetPortInfo
							 ->getAddress(RetSetEthernetPortInfo::STATIC |
										  RetSetEthernetPortInfo::SUBNET_MASK)
							 .c_str()));
	m_pGw1->setText(QString(m_pRetSetEthernetPortInfo
							 ->getAddress(RetSetEthernetPortInfo::STATIC |
										  RetSetEthernetPortInfo::GATEWAY)
							 .c_str()));
	m_pIp2->setText(QString(m_pRetSetEthernetPortInfo
							 ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
										  RetSetEthernetPortInfo::ADDRESS)
							 .c_str()));
	m_pSm2->setText(QString(m_pRetSetEthernetPortInfo
							 ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
										  RetSetEthernetPortInfo::SUBNET_MASK)
							 .c_str()));
	m_pGw2->setText(QString(m_pRetSetEthernetPortInfo
							 ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
										  RetSetEthernetPortInfo::GATEWAY)
							 .c_str()));
	m_pBj->setText(QString(m_pRetSetEthernetPortInfo->getBonjourName().c_str()));
	m_pMac->setText(QString(m_pRetSetEthernetPortInfo->getMacAddress().c_str()));

	int index = m_pMethodBox->findData(
		static_cast<int>(m_pRetSetEthernetPortInfo->getMethod()));
	if (index != -1) {// -1 for not found
		m_pMethodBox->setCurrentIndex(index);
		setStaticBoxEnabled(index);
	}
}

void EthernetInfoWidget::createConnections() {
	connect(iPAddressInputSignalMapper, SIGNAL(mapped(QObject *)), this,
			SLOT(editFinished(QObject *)));

	connect(m_pMethodBox, SIGNAL(activated(int)), this,
			SLOT(comboboxSelected(int)));
	connect(this, SIGNAL(staticBoxDisabled(bool)), m_pStaticBox,
			SLOT(setDisabled(bool)));

	connect(m_Ip1, SIGNAL(editingFinished()), iPAddressInputSignalMapper,
			SLOT(map()));
	iPAddressInputSignalMapper->setMapping(
		m_Ip1,
		new IPAddressInputMapper(m_Ip1, RetSetEthernetPortInfo::STATIC |
										  RetSetEthernetPortInfo::ADDRESS));
	connect(m_pSm1, SIGNAL(editingFinished()), iPAddressInputSignalMapper,
			SLOT(map()));
	iPAddressInputSignalMapper->setMapping(
		m_pSm1,
		new IPAddressInputMapper(m_pSm1, RetSetEthernetPortInfo::STATIC |
										  RetSetEthernetPortInfo::SUBNET_MASK));
	connect(m_pGw1, SIGNAL(editingFinished()), iPAddressInputSignalMapper,
			SLOT(map()));
	iPAddressInputSignalMapper->setMapping(
		m_pGw1,
		new IPAddressInputMapper(m_pGw1, RetSetEthernetPortInfo::STATIC |
										  RetSetEthernetPortInfo::GATEWAY));

	connect(this, SIGNAL(staticBoxDisabled(bool)), m_pStaticBox,
			SLOT(setDisabled(bool)));

	connect(r_mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateEthernetConfig()));
}

void EthernetInfoWidget::setStaticBoxEnabled(int selected) {
	bool disabled = (selected == 1);
	emit staticBoxDisabled(disabled);
}

bool EthernetInfoWidget::ipAddressControlsValid() {
	std::vector<IPAddressInput *>::iterator it;
	for (it = m_pValidateControls->begin(); it != m_pValidateControls->end(); ++it) {
		IPAddressInput *input = (*it);
		if (!input->getValid())
			return false;
	}
	return true;
}

void EthernetInfoWidget::comboboxSelected(int selected) {
	setStaticBoxEnabled(selected);
	m_pRetSetEthernetPortInfo->setMethod(
		static_cast<RetSetEthernetPortInfo::IPFlags>(
			m_pMethodBox->itemData(selected).Int));
	r_mUpdateTimer->start(1000);
}

void EthernetInfoWidget::editFinished(QObject *object) {
	IPAddressInputMapper *m = dynamic_cast<IPAddressInputMapper *>(object);
	std::cout << "Edit finished" << std::endl;
	if (m->input->getValid()) {
		m_pRetSetEthernetPortInfo->setAddress(m->flags,
										   m->input->text().toStdString());
		r_mUpdateTimer->start(1000);
	}
}

void EthernetInfoWidget::updateEthernetConfig() {
	if (ipAddressControlsValid()) {
		std::cout << "all controls valid" << std::endl;
		m_pRetSetEthernetPortInfo->execute();
	} else {
		std::cout << "at least one control invalid" << std::endl;
	}
}
