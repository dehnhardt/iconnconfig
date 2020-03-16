#ifndef PKSLIDER_H
#define PKSLIDER_H

#include <QSlider>
#include <memory>

#include "calc/scalecalc.h"

class PKSlider : public QSlider {
	Q_OBJECT

  public:
	PKSlider(QWidget *parent);
	PKSlider(QWidget *parent, std::shared_ptr<ScaleCalc> scaleCalc);

	void setScaleCalc(std::shared_ptr<ScaleCalc> scaleCalc);

	typedef enum { LINEAR, DECIBEL } ScaleType;

	void setMinimum(int val);
	void setMaximum(int val);

	void setValue(float val);
	void setValue(int val);

	bool getDebug() const;
	void setDebug(bool getDebug);

	ScaleType getScaleType() const;
	void setScaleType(const ScaleType &scaleType);

  private slots:
	void onValueChange(float val);

  signals:
	void valueChanged(int value);

	// QWidget interface
  protected:
	void paintEvent(QPaintEvent *event) override;

  private:
	int m_iHalfSliderHeight = 8;
	int m_iVal = 0;

	std::shared_ptr<ScaleCalc> m_pScaleCalc;

	bool m_bDebug = false;

	ScaleType m_scaleType = ScaleType::LINEAR;
	void paintLegend(QPainter &painter, int fontOffset, QRect contents);
};

#endif // PKSLIDER_H
