#ifndef PKSLIDER_H
#define PKSLIDER_H

#include <QSlider>

class PKSlider : public QSlider {
	Q_OBJECT

  public:
	PKSlider(QWidget *parent);

	typedef enum { LINEAR, LOGARITHM } ScaleType;

	void setMinimum(int val);
	void setMaximum(int val);
	void setResulution(int val);
	void setTickInterval(float val);

	void setValue(int val);

	bool getDebug() const;
	void setDebug(bool getDebug);

	ScaleType getScaleType() const;
	void setScaleType(const ScaleType &scaleType);

  private slots:
	void onValueChange(int val);

  signals:
	void valueChanged(int value);

	// QWidget interface
  protected:
	void paintEvent(QPaintEvent *event) override;

  private:
	float dbToAmplitude(float db);
	float amplitudeToDb(float amplitude);

	void calculateScale();

	int *m_pScales = nullptr;

	int translate(float dec, int min, int max);
	int m_iResolution = 0;
	float m_iRawTickInterval = 0;
	int m_iRawMinimum = 0;
	int m_iRawMaximum = 0;
	int m_iScaleDiff = 0;
	int m_iScaleTicks = 10;
	bool m_bDebug = false;
	int m_iHalfSliderHeight = 8;
	ScaleType m_scaleType = ScaleType::LINEAR;
	void paintLegendStep(QPainter &painter, int minVal, int val, int offset,
						 QRect contents);
};

#endif // PKSLIDER_H
