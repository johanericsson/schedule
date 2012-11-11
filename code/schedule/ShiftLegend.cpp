#include "stdafx.H"
#include "ShiftLegend.h"
#include "vectorhelper.h"

void operator<<(XMLNode& n,const Abbreviation& a)
{
	SerializeAttribute(n,_T("StartTime"),a.startTime);
	SerializeAttribute(n,_T("Hours"),a.hours);
	SerializeAttribute(n,_T("Abbreviation"),a.abbreviation);
}
void operator>>(const XMLNode& n,Abbreviation& a)
{
	SerializeAttribute(n,_T("StartTime"),a.startTime);
	SerializeAttribute(n,_T("Hours"),a.hours);
	SerializeAttribute(n,_T("Abbreviation"),a.abbreviation);
	if (a.abbreviation.GetLength() == 1)
		a.abbreviation += " ";
}

bool operator<(Abbreviation a1,Abbreviation a2)
{
	return ToString(a1) < ToString(a2);
}

CString ToString(Abbreviation a)
{
	CString str;
	str.Format(_T(": Start Time:%d Hours:%d"),a.startTime,
				a.hours);
	return a.abbreviation + str;
}

void ShiftLegend::AddAbbreviation(Abbreviation a)
{
	m_abbreviations.push_back(a);
	sort(m_abbreviations.begin(),m_abbreviations.end());
}
int ShiftLegend::Size() const
{
	return m_abbreviations.size();
}
void ShiftLegend::Remove(int i)
{
	m_abbreviations.erase(m_abbreviations.begin() + i);
}

Abbreviation& ShiftLegend::operator[](int i)
{
	return m_abbreviations[i];
}

const Abbreviation& ShiftLegend::operator[](int i) const
{
	return m_abbreviations[i];
}

struct IsSame
{
	Abbreviation m_a;
	IsSame(Abbreviation a):m_a(a){}
	bool operator()(Abbreviation b)
	{
		if (m_a.startTime == b.startTime)
			if (m_a.hours == b.hours)
				return true;
		return false;
	}
};

CString ShiftLegend::GetAbbreviation(int startTime,int hours)const
{
	vector<Abbreviation>::const_iterator i =
		find_if(m_abbreviations.begin(),m_abbreviations.end(),
		IsSame(Abbreviation(startTime,hours,_T(""))));
	
	if (i==m_abbreviations.end())
		return _T("Wo");
	return i->abbreviation;
}

void operator<<(XMLNode& n,const ShiftLegend& s)
{
	Serialize(n,_T("Abbreviations"),s.m_abbreviations);
}
void operator>>(const XMLNode& n,ShiftLegend& s)
{
	Serialize(n,_T("Abbreviations"),s.m_abbreviations);
}