#ifndef DBCALC_H
#define DBCALC_H

#include "scalecalc.h"

class DbCalc : public ScaleCalc {
  public:
	DbCalc(float decMin, float decMax, float logMin, float logMax);
	virtual ~DbCalc() override;

	float encode(float db) override;
	float encodeScale(float db) override;
	float decode(float dec) override;

	float decToDb(float dec) const;
	float dbToDec(float db) const;

	float mapDec(float dec) const;
	float mapDecScale(float dec) const;
	float unmapDec(float dec) const;

	float ticks() const;
	void setTicks(float fTicks);

  protected: // methods
	void init() override;

  protected: // variables
	float m_fLogMin = 0;
	float m_fLogMax = 0;

  private: // variables
	float m_fDecVirtMin = 0;
	float m_fDecVirtMax = 0;
	float m_fDecVirtRange = 0;

	float m_fLogRangeRel = 0;
	float m_fLogRange = 0;
	float m_fLogMap = 0;

	float m_fTicks = 0;

	template <typename T> int sgn(T val) const {
		return (T(0) < val) - (val < T(0));
	}
};

#endif // DBCALC_H
