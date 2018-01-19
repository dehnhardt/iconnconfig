#ifndef ETHERNETINFOWIDGET_H
#define ETHERNETINFOWIDGET_H

#include "../sysex/retsetethernetportinfo.h"
#include "controls/ipaddressinput.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QTimer>
#include <QWidget>

class IPAddressInputMapper : public QObject
{
public:
	IPAddressInputMapper(IPAddressInput *input, int flags)
		: input(input), flags(flags) {}
	IPAddressInput *input;
	int flags;
};

class EthernetInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit EthernetInfoWidget(QWidget *parent,
								RetSetEthernetPortInfo *m_pRetSetEthernetPortInfo);

	virtual ~EthernetInfoWidget();

signals:
	void staticBoxDisabled(bool disabled);

public slots:
	void comboboxSelected(int selected);

private slots:
	void editFinished(QObject *object);
	void updateEthernetConfig();

private:
	void createWidgets();
	void setupWidgets();
	void setupLayout();
	void setData();
	void createConnections();
	void setStaticBoxEnabled(int selected);
	bool ipAddressControlsValid();

	// Members
private:
	RetSetEthernetPortInfo *m_pRetSetEthernetPortInfo = 0;
	QTimer *r_mUpdateTimer = 0;

	QSignalMapper *iPAddressInputSignalMapper = 0;

	QRegExpValidator *regValidator = 0;

	QGridLayout *m_pRootLayout = 0;
	QGridLayout *m_pStaticLayout = 0;
	QGridLayout *m_pDhcpLayout = 0;
	QGridLayout *m_pInfoLayout = 0;

	QLabel *m_pTl   = 0;
	QLabel *m_pIpl1 = 0;
	QLabel *m_pSml1 = 0;
	QLabel *m_pGwl1 = 0;
	QLabel *m_PIpl2 = 0;
	QLabel *m_pSml2 = 0;
	QLabel *m_pGwl2 = 0;
	QLabel *m_pBjl  = 0;
	QLabel *m_pMacl = 0;

	QComboBox *m_pMethodBox = 0;

	QGroupBox *m_pStaticBox = 0;
	QGroupBox *m_pDhcpBox = 0;
	QGroupBox *m_pInfoBox = 0;

	IPAddressInput *m_Ip1 = 0;
	IPAddressInput *m_pSm1 = 0;
	IPAddressInput *m_pGw1 = 0;

	QLineEdit *m_pIp2 = 0;
	QLineEdit *m_pSm2 = 0;
	QLineEdit *m_pGw2 = 0;
	QLineEdit *m_pBj = 0;
	QLineEdit *m_pMac = 0;

	QWidget *m_pEmpty = 0;

	std::vector<IPAddressInput *> *m_pValidateControls;

};

#endif // ETHERNETINFOWIDGET_H
