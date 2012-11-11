#include "stdafx.h"
#include "data.h"
#include "daterange.h"
#include "fixedschedule.h"
#include "vectorhelper.h"


CArchive& operator<<(CArchive& ar,const Name& n)
{
	ar << n.First();
	ar << n.Last();
	return ar;
}

CArchive& operator>>(CArchive& ar,Name& n)
{
	ar >> n.m_first;
	ar >> n.m_last;
	return ar;
}


CArchive& operator<<(CArchive& ar,const Data& d)
{
	ar << d.m_nurses;
	ar << d.m_ShiftsTemplate;
	ar << d.m_legend;
	ar << d.m_assignments;
	return ar;
}

CArchive& operator>>(CArchive& ar,Data& d)
{
	ar >> d.m_nurses;
	ar >> d.m_ShiftsTemplate;
	ar >> d.m_legend;
	ar >> d.m_assignments;

	return ar;
}

void operator<<(CArchive& ar,const DateRange& d)
{
	ar << d.m_startDate;
	ar << d.m_weeks;
}
void operator>>(CArchive& ar,DateRange& d)
{
	ar >> d.m_startDate;
	ar >> d.m_weeks;
}

void operator<<(CArchive& ar,const FixedScheduleEntry& f)
{
	ar << f.m_week;
	ar << f.m_day;
	ar << f.m_startTime;
	ar << f.m_hours;
}
void operator>>(CArchive& ar,FixedScheduleEntry& f)
{
	ar >> f.m_week;
	ar >> f.m_day;
	ar >> f.m_startTime;
	ar >> f.m_hours;
}

void operator<<(CArchive& ar,const FixedSchedule& f)
{
	ar << f.m_bOnlyAvailable;
	ar << f.m_fixedScheduleEntries;
	ar << f.m_numberOfWeeks;
	ar << f.m_offset;
}
void operator>>(CArchive& ar,FixedSchedule& f)
{
	ar >> f.m_bOnlyAvailable;
	ar >> f.m_fixedScheduleEntries;
	ar >> f.m_numberOfWeeks;
	ar >> f.m_offset;
}