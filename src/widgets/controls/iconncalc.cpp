#include "iconncalc.h"

IConnCalc::IConnCalc(float decMin, float decMax, float logMin, float logMax,
					 float ticks)
	: DbCalc(decMin, decMax, logMin / ticks, logMax / ticks) {
	setTicks(ticks);
	init();
}

void IConnCalc::init() {
	DbCalc::init();
	if ((m_fLogMin == -63) && (m_fLogMax == 0)) {
		setVScaleValues(
			std::vector<float>{-63, -50, -40, -30, -20, -10, -5, 0});
	} else if ((m_fLogMin == 0) && (m_fLogMax == 60)) {
		setVScaleValues(std::vector<float>{0, 5, 10, 20, 30, 40, 50, 60});
	} else if ((m_fLogMin == -80) && (m_fLogMax == 6)) {
		setVScaleValues(
			std::vector<float>{-80, -40, -30, -20, -10, -6, -3, 0, 3, 6});
	}
}
