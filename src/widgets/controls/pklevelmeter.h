#ifndef PKLEVELMETER_H
#define PKLEVELMETER_H

#include <QObject>
#include <QProgressBar>

class PKLevelMeter : public QProgressBar {
  public:
	PKLevelMeter(QWidget *parent = nullptr);

	// QWidget interface
  protected:
	void paintEvent(QPaintEvent *event) override;
};

#endif // PKLEVELMETER_H
