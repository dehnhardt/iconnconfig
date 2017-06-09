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

class IPAddressInputMapper : public QObject {
public:
	IPAddressInputMapper(IPAddressInput *input, int flags)
			: input(input), flags(flags) {}
	IPAddressInput *input;
	int flags;
};

class EthernetInfoWidget : public QWidget {
  Q_OBJECT
public:
  explicit EthernetInfoWidget(QWidget *parent,
                              RetSetEthernetPortInfo *retSetEthernetPortInfo);

  virtual ~EthernetInfoWidget();

signals:
  void staticBoxDisabled(bool disabled);

public slots:
  void comboboxSelected(int selected);

private slots:
	void editFinished(QObject *object);
	void updateEthernetConfig();

private:
  RetSetEthernetPortInfo *retSetEthernetPortInfo = 0;
	QTimer *updateTimer = 0;

	QSignalMapper *iPAddressInputSignalMapper = 0;

  QRegExpValidator *regValidator = 0;

  QGridLayout *lo = 0;
  QGridLayout *staticLayout = 0;
  QGridLayout *dhcpLayout = 0;
  QGridLayout *infoLayout = 0;

  QLabel *tl = 0;
  QLabel *ipl1 = 0;
  QLabel *sml1 = 0;
  QLabel *gwl1 = 0;
  QLabel *ipl2 = 0;
  QLabel *sml2 = 0;
  QLabel *gwl2 = 0;
  QLabel *bjl = 0;
  QLabel *macl = 0;

  QComboBox *methodBox = 0;

  QGroupBox *staticBox = 0;
  QGroupBox *dhcpBox = 0;
  QGroupBox *infoBox = 0;

	IPAddressInput *ip1 = 0;
	IPAddressInput *sm1 = 0;
	IPAddressInput *gw1 = 0;
  QLineEdit *ip2 = 0;
  QLineEdit *sm2 = 0;
  QLineEdit *gw2 = 0;
  QLineEdit *bj = 0;
  QLineEdit *mac = 0;

  QWidget *empty = 0;

	std::vector<IPAddressInput *> *validateControls;

private:
  void createWidgets();
  void setupWidgets();
  void setupLayout();
  void setData();
  void createConnections();
  void setStaticBoxEnabled(int selected);
	bool ipAddressControlsValid();
};

#endif // ETHERNETINFOWIDGET_H
