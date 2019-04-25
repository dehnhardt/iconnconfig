#ifndef PKLEVELMETER_H
#define PKLEVELMETER_H

#include <QObject>
#include <QProgressBar>

class PKLevelMeter : public QProgressBar {
  public:
	PKLevelMeter(QWidget *parent = nullptr);
};

#endif // PKLEVELMETER_H
