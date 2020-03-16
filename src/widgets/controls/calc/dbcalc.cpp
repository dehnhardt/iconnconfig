#include "dbcalc.h"
#include "math.h"

DbCalc::DbCalc(float decMin, float decMax, float logMin, float logMax)
	: ScaleCalc(decMin, decMax), m_fLogMin(logMin), m_fLogMax(logMax) {
	init();
}

DbCalc::~DbCalc(){};

float DbCalc::encode(float db) { return mapDec(dbToDec(db / m_fTicks)); }

float DbCalc::encodeScale(float db) { return mapDecScale(dbToDec(db)); }

float DbCalc::decode(float dec) { return decToDb(unmapDec(dec)) * m_fTicks; }

void DbCalc::init() {

	m_fDecRange = m_fDecMax - m_fDecMin;
	m_fLogRange = m_fLogMax - m_fLogMin;

	m_fDecVirtMin = dbToDec(m_fLogMin);
	m_fDecVirtMax = dbToDec(m_fLogMax);
	m_fDecVirtRange = m_fDecVirtMax - m_fDecVirtMin;

	m_fLogMap = m_fDecRange / m_fDecVirtRange;
}

float DbCalc::ticks() const { return m_fTicks; }

void DbCalc::setTicks(float fTicks) { m_fTicks = fTicks; }

float DbCalc::decToDb(float dec) const {
	float db;

	if (dec > m_fDecVirtMax)
		dec = m_fDecVirtMax;
	if (dec < m_fDecVirtMin)
		dec = m_fDecVirtMin;

	if (abs(dec) == 0.f) {
		db = 0.f;
	} else if (abs(dec) < 24.f) {
		db = dec * 5.f / 12.f;
	} else if (abs(dec) < 60.f) {
		db = ((abs(dec) - 24.f) / 1.2f + 10.f) * sgn(dec);
	} else if (abs(dec) < 72.f) {
		db = ((abs(dec) - 60.f) / 0.6f + 40.f) * sgn(dec);
	} else if (dec < 76.f) {
		db = ((abs(dec) - 72.f) / (1.f / 35.f) + 60.f) * sgn(dec);
	} else
		db = -200.;
	return db;
}

float DbCalc::unmapDec(float dec) const {
	return (dec - m_fDecMin) / m_fLogMap + m_fDecVirtMin;
}

float DbCalc::dbToDec(float db) const {
	float dec;
	if (db > m_fLogMax) {
		db = m_fLogMax;
	}
	if (db < m_fLogMin) {
		db = m_fLogMin;
	}
	if (abs(db) <= 10.f) {
		dec = 12.f / 5.f * db;
	} else if (abs(db) < 40.f) {
		dec = (24.f + 1.2f * (abs(db) - 10.f)) * sgn(db);
	} else if (abs(db) < 60.f) {
		dec = (60.f + 0.6f * (abs(db) - 40.f)) * sgn(db);
	} else if (abs(db) <= 200.f) {
		dec = (72.f + 1.f / 35.f * (abs(db) - 60.f)) * sgn(db);
	} else
		dec = m_fDecRange;
	return dec;
}

float DbCalc::mapDecScale(float dec) const {
	return (dec - m_fDecVirtMin) * m_fLogMap;
}

float DbCalc::mapDec(float dec) const {
	return (dec - m_fDecVirtMin) * m_fLogMap + m_fDecMin;
}
