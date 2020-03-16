#include "calclog.h"
#include "math.h"


CalcLog::CalcLog(float fMin, float fMax) :
	  m_fMin(fMin), m_fMax(fMax)
{
	m_fScale = fMax - fMin;

	m_fLogmin = log10(abs(fMin+1)) *sgn(fMin);
	m_fLogmax = log10(abs(fMax+1)) *sgn(fMax);
	m_fLogdiff = m_fLogmax - m_fLogmin;
	m_fFactor = m_fScale / m_fLogdiff;

}

float CalcLog::calc(float fVal)
{
	float fMappedVal =  m_bInputUnmapped ? fVal * m_fScale:fVal;
	float ampI = log10(abs(fMappedVal)+1) * sgn(fMappedVal);
	float scaleVal = ampI * m_fFactor;
	if( scaleVal < m_fMin )
		scaleVal = m_fMin;
	else if( scaleVal > m_fMax )
		scaleVal = m_fMax;
	return scaleVal;
}

bool CalcLog::inputUnmapped() const
{
	return m_bInputUnmapped;
}

void CalcLog::setInputUnmapped(bool bInputUnmapped)
{
	m_bInputUnmapped = bInputUnmapped;
}

float CalcLog::fMin() const
{
	return m_fMin;
}

float CalcLog::fMax() const
{
	return m_fMax;
}

float CalcLog::fMinToMap() const
{
	return m_fMinToMap;
}

float CalcLog::fMaxToMap() const
{
	return m_fMaxToMap;
}


