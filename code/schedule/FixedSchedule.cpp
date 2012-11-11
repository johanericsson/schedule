#include "stdafx.h"
#include "FixedSchedule.h"
#include "WeekNames.h"
#include "myexception.h"
#include "VectorHelper.h"
#include "Version.h"

FixedScheduleEntry::
FixedScheduleEntry(int week,int day,int startTime,int hours):
m_week(week),
m_day(day),
m_startTime(startTime),
m_hours(hours)
{
	CheckWeekdayNumber(day);
}

CString FixedScheduleEntry::ToString() const
{
	CString str;
	str.Format(_T("Week:%d Weekday:"),m_week);
	str += WeekName(m_day);
	CString second;
	second.Format(_T(" StartTime:%d Hours:%d"),m_startTime,m_hours);
	return str + second;
}

int FixedScheduleEntry::Week() const
{
	return m_week;
}

int FixedScheduleEntry::Day() const
{
	return m_day;
}

int FixedScheduleEntry::StartTime() const
{
	return m_startTime;
}

int FixedScheduleEntry::Hours() const
{
	return m_hours;
}

bool operator<(const FixedScheduleEntry& f1,
			   const FixedScheduleEntry& f2)
{
	return (f1.Week() < f2.Week());
}

void operator<<(XMLNode& ar,const FixedScheduleEntry& f)
{
	SerializeAttribute(ar,_T("Week"),f.m_week);
	SerializeAttribute(ar,_T("Day"),f.m_day);
	SerializeAttribute(ar,_T("StartTime"),f.m_startTime);
	SerializeAttribute(ar,_T("Hours"),f.m_hours);
}
void operator>>(const XMLNode& ar,FixedScheduleEntry& f)
{
	SerializeAttribute(ar,_T("Week"),f.m_week);
	SerializeAttribute(ar,_T("Day"),f.m_day);
	SerializeAttribute(ar,_T("StartTime"),f.m_startTime);
	SerializeAttribute(ar,_T("Hours"),f.m_hours);
}


FixedSchedule::FixedSchedule():m_numberOfWeeks(1),
m_offset(0),
m_bOnlyAvailable(false)
{
}

void FixedSchedule::SetNumberOfWeeks(int i)
{

	if (i<1)
		i = 1;
	m_numberOfWeeks = i;
}

int FixedSchedule::GetNumberOfWeeks() const
{
	return m_numberOfWeeks;
}	

FixedScheduleEntry& FixedSchedule::operator[](int i)
{
	return m_fixedScheduleEntries[i];
}

const FixedScheduleEntry& FixedSchedule::operator[](int i)const
{
	return m_fixedScheduleEntries[i];
}

void FixedSchedule::Add(FixedScheduleEntry entry)
{
	m_fixedScheduleEntries.push_back(entry);
	std::sort(m_fixedScheduleEntries.begin(),
		m_fixedScheduleEntries.end());
}

int FixedSchedule::Size() const
{
	int i=0;
	for (i=0;i<m_fixedScheduleEntries.size();i++)
	{
		if (m_fixedScheduleEntries[i].Week() > 
			GetNumberOfWeeks())
			return i;
	}
	return i;
}

void FixedSchedule::Remove(int i)
{
	m_fixedScheduleEntries.erase(
		m_fixedScheduleEntries.begin() + i);
}

void FixedSchedule::SetOffset(int weeks)
{
	m_offset = weeks;
}
int FixedSchedule::GetOffset() const
{
	return m_offset;
}
static bool MatchWeek(int dateWeek,
					  int week,
					  int rotationWeeks,
					  int offset)
{
	int relativeWeek = (dateWeek+offset) % (rotationWeeks);
	return (relativeWeek == week);
}

bool
FixedSchedule::IsWorkingOn( CTime date,int entryNumber)const
{
	const FixedScheduleEntry& entry = (*this)[entryNumber];
	if (entry.Day() != (date.GetDayOfWeek()-1))
		return false;
	
	// first sunday of the millenium
	CTime firstOfMil(2001,1,7,0,0,0,0);
			CTimeSpan spanFromFirstMil = date - firstOfMil;
			double week = (double)(spanFromFirstMil.GetTimeSpan())/(24*3600);
	week/=7;
	if (MatchWeek(static_cast<int>(spanFromFirstMil.GetDays()/7),
				entry.Week()
				,this->GetNumberOfWeeks()
				,this->GetOffset()))
	{
		return true;
	}
	return false;
}

void operator<<(XMLNode& n,const FixedSchedule& f)
{
	SerializeAttribute(n,_T("IsOnlyAvailable"),f.m_bOnlyAvailable);
	Serialize(n,_T("Entries"),f.m_fixedScheduleEntries);
	SerializeAttribute(n,_T("NumberOfWeeks"),f.m_numberOfWeeks);
	SerializeAttribute(n,_T("Offset"),f.m_offset);
}
void operator>>(const XMLNode& n,FixedSchedule& f)
{
	SerializeAttribute(n,_T("IsOnlyAvailable"),f.m_bOnlyAvailable);
	Serialize(n,_T("Entries"),f.m_fixedScheduleEntries);
	SerializeAttribute(n,_T("NumberOfWeeks"),f.m_numberOfWeeks);
	SerializeAttribute(n,_T("Offset"),f.m_offset);
}