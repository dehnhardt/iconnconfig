#ifndef CALCLOG_H
#define CALCLOG_H


class CalcLog
{
public:
	CalcLog(float fMin, float fMax);

public: //getter
	bool inputUnmapped() const;
	float fMin() const;
	float fMax() const;
	float fMinToMap() const;
	float fMaxToMap() const;

public: //setter
	void setInputUnmapped(bool bInputUnmapped);

public: //functions
	float calc( float fVal );


private:
	bool m_bInputUnmapped = false;

	float m_fMin;
	float m_fMax;

	float m_fMinToMap;
	float m_fMaxToMap;

	float m_fScale;
	float m_fFactor;

	float m_fLogmin;
	float m_fLogmax;
	float m_fLogdiff;

	template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

};

#endif // CALCLOG_H
