#ifndef PKSLIDER_H
#define PKSLIDER_H

#include <QSlider>

class PKSlider : public QSlider {
	Q_OBJECT

  public:
	typedef enum { LINEAR, DECIBEL } ScaleType;
	typedef enum { POSITIV = 1, NEGATIV, NEGPOS } ScaleSpan;
	PKSlider(QWidget *parent);

	void setMinimum(int val);
	void setMaximum(int val);
	void setResulution(float val);
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
	template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
	float dbToAmplitude(float db);
	float amplitudeToDb(float amplitude);

	int *m_pScales = nullptr;

	int translate(float dec, int min, int max);
	float m_iResolution = 1;
	float m_iRawTickInterval = 0;
	int m_iRawMinimum = 0;
	int m_iRawMaximum = 0;
	int m_iScaleDiff = 0;
	int m_iScaleTicks = 10;
	bool m_bDebug = false;
	bool m_bDebug2 = false;
	int m_iHalfSliderHeight = 8;
	int m_iMinimalLegendDistance = 10;
	ScaleType m_scaleType = ScaleType::LINEAR;
	ScaleSpan m_scaleSpan = ScaleSpan::POSITIV;

	void setScaleSpan();
	void paintLegend(QPainter &painter, int offset, QRect contents);
	void paintLegendLine(QPainter &painter, QRect contents, int offset, int val,
						 int xPos);
};

#endif // PKSLIDER_H
