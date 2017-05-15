#include "ethernetinfowidget.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>

EthernetInfoWidget::EthernetInfoWidget(
    QWidget *parent, RetSetEthernetPortInfo *retSetEthernetPortInfo)
    : QWidget(parent), retSetEthernetPortInfo(retSetEthernetPortInfo) {
  QGridLayout *lo = new QGridLayout();
  QRegExp rx("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
  QRegExpValidator regValidator(rx, 0);

  if (retSetEthernetPortInfo) {
    QLabel *tl = new QLabel(tr("Address Type"));
    lo->addWidget(tl, 0, 0);
    QComboBox *methodBox = new QComboBox(this);
    connect(methodBox, SIGNAL(activated(int)), this,
            SLOT(comboboxSelected(int)));
    methodBox->addItem(tr("Static"),
                       QVariant((int)RetSetEthernetPortInfo::STATIC));
    methodBox->addItem(tr("Dynamic"),
                       QVariant((int)RetSetEthernetPortInfo::DYNAMIC));
    lo->addWidget(methodBox, 0, 1);
    QGroupBox *staticBox = new QGroupBox(tr("Static Address"), this);
    connect(this, &EthernetInfoWidget::staticBoxDisabled, staticBox,
            &QGroupBox::setDisabled);

    QGridLayout *staticLayout = new QGridLayout();
    QLabel *ipl1 = new QLabel(tr("Static IP"), staticBox);
    QLineEdit *ip1 = new QLineEdit(staticBox);
    ip1->setValidator(&regValidator);
    ip1->setText(QString(retSetEthernetPortInfo
                             ->getAddress(RetSetEthernetPortInfo::STATIC |
                                          RetSetEthernetPortInfo::ADDRESS)
                             .c_str()));
    staticLayout->addWidget(ipl1, 0, 0);
    staticLayout->addWidget(ip1, 0, 1);

    QLabel *sml1 = new QLabel(tr("Subnet Mask"), staticBox);
    QLineEdit *sm1 = new QLineEdit(staticBox);
    sm1->setValidator(&regValidator);
    sm1->setText(QString(retSetEthernetPortInfo
                             ->getAddress(RetSetEthernetPortInfo::STATIC |
                                          RetSetEthernetPortInfo::SUBNET_MASK)
                             .c_str()));
    staticLayout->addWidget(sml1, 1, 0);
    staticLayout->addWidget(sm1, 1, 1);

    QLabel *gwl1 = new QLabel(tr("Gatewey"), staticBox);
    QLineEdit *gw1 = new QLineEdit(staticBox);
    gw1->setValidator(&regValidator);
    gw1->setText(QString(retSetEthernetPortInfo
                             ->getAddress(RetSetEthernetPortInfo::STATIC |
                                          RetSetEthernetPortInfo::GATEWAY)
                             .c_str()));
    staticLayout->addWidget(gwl1, 2, 0);
    staticLayout->addWidget(gw1, 2, 1);

    staticBox->setLayout(staticLayout);
    lo->addWidget(staticBox, 1, 0, 1, 2);

    QGroupBox *dhcpBox = new QGroupBox(tr("DHCP Address"), this);
    dhcpBox->setEnabled(false);
    QGridLayout *dhcpLayout = new QGridLayout();

    QLabel *ipl2 = new QLabel(tr("Static IP"), this);
    QLineEdit *ip2 = new QLineEdit(this);
    ip2->setValidator(&regValidator);
    ip2->setText(QString(retSetEthernetPortInfo
                             ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
                                          RetSetEthernetPortInfo::ADDRESS)
                             .c_str()));
    dhcpLayout->addWidget(ipl2, 0, 0);
    dhcpLayout->addWidget(ip2, 0, 1);

    QLabel *sml2 = new QLabel(tr("Subnet Mask"), this);
    QLineEdit *sm2 = new QLineEdit(this);
    sm2->setValidator(&regValidator);
    sm2->setText(QString(retSetEthernetPortInfo
                             ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
                                          RetSetEthernetPortInfo::SUBNET_MASK)
                             .c_str()));
    dhcpLayout->addWidget(sml2, 1, 0);
    dhcpLayout->addWidget(sm2, 1, 1);

    QLabel *gwl2 = new QLabel(tr("Gatewey"), this);
    QLineEdit *gw2 = new QLineEdit(this);
    gw2->setValidator(&regValidator);
    gw2->setText(QString(retSetEthernetPortInfo
                             ->getAddress(RetSetEthernetPortInfo::DYNAMIC |
                                          RetSetEthernetPortInfo::GATEWAY)
                             .c_str()));
    dhcpLayout->addWidget(gwl2, 2, 0);
    dhcpLayout->addWidget(gw2, 2, 1);
    dhcpBox->setLayout(dhcpLayout);
    lo->addWidget(dhcpBox, 2, 0, 1, 2);
    int index = methodBox->findData((int)retSetEthernetPortInfo->getMethod());
    if (index != -1) { // -1 for not found
      methodBox->setCurrentIndex(index);
      comboboxSelected(index);
    }
  }
  setLayout(lo);
}

void EthernetInfoWidget::comboboxSelected(int selected) {
  bool disabled = (selected == 1);
  emit staticBoxDisabled(disabled);
}
