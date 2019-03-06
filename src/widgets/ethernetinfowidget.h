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

class EthernetInfoWidget : public QWidget {
	Q_OBJECT
  public:
	explicit EthernetInfoWidget(
		QWidget *parent, RetSetEthernetPortInfo *m_pRetSetEthernetPortInfo);

	virtual ~EthernetInfoWidget();

  signals:
	void staticBoxDisabled(bool disabled);

  public slots:
	void comboboxSelected(int selected);

  private slots:
	void editFinished(IPAddressInput *input, int flags);
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
	RetSetEthernetPortInfo *m_pRetSetEthernetPortInfo = nullptr;
	QTimer *r_mUpdateTimer = nullptr;

	QSignalMapper *iPAddressInputSignalMapper = nullptr;

	QRegExpValidator *regValidator = nullptr;

	QGridLayout *m_pRootLayout = nullptr;
	QGridLayout *m_pStaticLayout = nullptr;
	QGridLayout *m_pDhcpLayout = nullptr;
	QGridLayout *m_pInfoLayout = nullptr;

	QLabel *m_pTl = nullptr;
	QLabel *m_pIpl1 = nullptr;
	QLabel *m_pSml1 = nullptr;
	QLabel *m_pGwl1 = nullptr;
	QLabel *m_PIpl2 = nullptr;
	QLabel *m_pSml2 = nullptr;
	QLabel *m_pGwl2 = nullptr;
	QLabel *m_pBjl = nullptr;
	QLabel *m_pMacl = nullptr;

	QComboBox *m_pMethodBox = nullptr;

	QGroupBox *m_pStaticBox = nullptr;
	QGroupBox *m_pDhcpBox = nullptr;
	QGroupBox *m_pInfoBox = nullptr;

	IPAddressInput *m_Ip1 = nullptr;
	IPAddressInput *m_pSm1 = nullptr;
	IPAddressInput *m_pGw1 = nullptr;

	QLineEdit *m_pIp2 = nullptr;
	QLineEdit *m_pSm2 = nullptr;
	QLineEdit *m_pGw2 = nullptr;
	QLineEdit *m_pBj = nullptr;
	QLineEdit *m_pMac = nullptr;

	QWidget *m_pEmpty = nullptr;

	std::vector<IPAddressInput *> *m_pValidateControls;
};

#endif // ETHERNETINFOWIDGET_H
