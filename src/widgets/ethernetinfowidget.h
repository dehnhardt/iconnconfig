#ifndef ETHERNETINFOWIDGET_H
#define ETHERNETINFOWIDGET_H

#include "../sysex/retsetethernetportinfo.h"

#include <QWidget>

class EthernetInfoWidget : public QWidget {
  Q_OBJECT
public:
  explicit EthernetInfoWidget(QWidget *parent,
                              RetSetEthernetPortInfo *retSetEthernetPortInfo);

signals:

public slots:

private:
  RetSetEthernetPortInfo *retSetEthernetPortInfo = 0;
};

#endif // ETHERNETINFOWIDGET_H
