#pragma once

#include "DateRange.h"
#include "Data.h"
class XMLNode;
class Period
{
public:
	Period(){}
	Period(const DateRange& dateRange):
	  m_dateRange(dateRange)
	  {
	  }
	void Import(const Period& oldPeriod);
	DateRange m_dateRange;
	Data m_data;

	static CString GetNodeTitle()
	{
		return _T("Period");
	}

	friend void operator<<(XMLNode& node, const Period& p);
	friend void operator>>(const XMLNode& node,Period& p);

};

void operator<<(XMLNode& node, const Period& p);
void operator>>(const XMLNode& node,Period& p);

class Periods
{
public:
	const Period& operator[](int i) const {return m_periods[i];}
	Period& operator[](int i) {return m_periods[i];}
	int Size() const {return m_periods.size();}
	void Add(const Period& p);
	Period& GetCurrentPeriod();
	Periods();
	bool IsEmpty() const;
	void Remove(int i)
	{
		ASSERT(i<m_periods.size());
		m_periods.erase(m_periods.begin() + i);
		if (i<=m_current)
			m_current--;
	}
	int GetCurrent() const
	{
		return m_current;

	}
	void SetCurrent(int current)
	{
		ASSERT(current < m_periods.size());
		m_current = current;
	}
private:
	int m_current;
	vector<Period> m_periods;

	friend void operator<<(XMLNode& node, const Periods& p);
	friend void operator>>(const XMLNode& node,Periods& p);

};

void operator<<(XMLNode& node, const Periods& p);
void operator>>(const XMLNode& node,Periods& p);