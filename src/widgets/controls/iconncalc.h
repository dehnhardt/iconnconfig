#ifndef ICONNCALC_H
#define ICONNCALC_H

#include "calc/dbcalc.h"

class IConnCalc : public DbCalc {
  public:
	IConnCalc(float decMin, float decMax, float logMin, float logMax,
			  float ticks);

	// ScaleCalc interface
  protected:
	virtual void init() override;
};

#endif // ICONNCALC_H
