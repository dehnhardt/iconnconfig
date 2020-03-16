#ifndef SCALECALC_H
#define SCALECALC_H

#include <vector>

class ScaleCalc {
  public:
	ScaleCalc(float decMin, float decMax);
	virtual ~ScaleCalc();
	virtual float encode(float val);
	virtual float encodeScale(float val);
	virtual float decode(float val);

	virtual float minAllowedValue();
	virtual float maxAllowedValue();

	float decMin() const;
	float decMax() const;
	float decRange() const;

	void setDecMin(float decMin);
	void setDecMax(float decMax);

	std::vector<float> vScaleValues() const;
	void setVScaleValues(const std::vector<float> &vScaleValues);

  protected:
	virtual void init();
	float m_fDecMin = 0;
	float m_fDecMax = 0;
	float m_fDecRange = 0;
	std::vector<float> m_vScaleValues;
};

#endif // SCALECALC_H
