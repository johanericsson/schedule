#include "stdafx.h"
#include "DateRange.h"
#include "vectorhelper.h"
#include "XMLNode.h"

DateRange::DateRange(const CTime& startDate,int weeks):
m_startDate(startDate),
m_weeks(weeks)
{
}

CTime 
DateRange::GetStartDate()const
{
	return m_startDate;
}

int 
DateRange::GetNumberOfWeeks() const
{
	return m_weeks;
}

CTime 
DateRange::GetEndDate() const
{
	CTimeSpan span(m_weeks*7,0,0,0);
	return m_startDate + span;
}

CString 
DateRange::ToString() const
{
	CString start = m_startDate.Format(_T("%b %d %Y"));
	CString numberOfWeeks;
	numberOfWeeks.Format(_T(" Weeks: %d"),m_weeks);
	return start + numberOfWeeks;
}

void operator<<(XMLNode& node,const DateRange& d)
{
	int year = d.m_startDate.GetYear();
	int month = d.m_startDate.GetMonth();
	int day = d.m_startDate.GetDay();
	node.AddAttribute(_T("Year"),year);
	node.AddAttribute(_T("Month"),month);
	node.AddAttribute(_T("Day"),day);
	ASSERT(d.m_startDate == CTime(year,month,day,0,0,0,0));
	node.AddAttribute(_T("Weeks"),d.m_weeks);
}
void operator>>(const XMLNode& node,DateRange& d)
{
	node.GetAttribute(_T("Weeks"),d.m_weeks);

	int year;
	int month;
	int day;
	node.GetAttribute(_T("Year"),year);
	node.GetAttribute(_T("Month"),month);
	node.GetAttribute(_T("Day"),day);
	d.m_startDate = CTime(year,month,day,0,0,0,0); 
}

