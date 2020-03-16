#include "scalecalc.h"

ScaleCalc::ScaleCalc(float decMin, float decMax)
	: m_fDecMin(decMin), m_fDecMax(decMax) {
	init();
}

ScaleCalc::~ScaleCalc() {}

float ScaleCalc::encode(float val) { return val; }

float ScaleCalc::encodeScale(float val) { return val; }

float ScaleCalc::decode(float val) { return val; }

float ScaleCalc::minAllowedValue() { return decMin(); }

float ScaleCalc::maxAllowedValue() { return decMax(); }

float ScaleCalc::decMin() const { return m_fDecMin; }

float ScaleCalc::decMax() const { return m_fDecMax; }

float ScaleCalc::decRange() const { return m_fDecRange; }

void ScaleCalc::setDecMin(float decMin) {
	m_fDecMin = decMin;
	init();
}

void ScaleCalc::setDecMax(float decMax) {
	m_fDecMax = decMax;
	init();
}

std::vector<float> ScaleCalc::vScaleValues() const { return m_vScaleValues; }

void ScaleCalc::setVScaleValues(const std::vector<float> &vScaleValues) {
	m_vScaleValues = vScaleValues;
}

void ScaleCalc::init() { m_fDecRange = m_fDecMax - m_fDecMin; };
