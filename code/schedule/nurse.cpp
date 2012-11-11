#include "stdafx.h"
#include "Nurse.h"
#include "VectorHelper.h"
#include "shiftlegend.h"

Nurse::Nurse(Name name,int dailyHours,int weeklyHours,
			 int startTime):
m_name(name),
m_dailyHours(dailyHours),
m_weeklyHours(weeklyHours),
m_startTime(startTime),
m_classification(_T("XXXX")),
m_payPeriodHours(-1)
{
}

void Nurse::SetStartTime(int startTime)
{
	m_startTime = startTime;
}
int Nurse::GetStartTime() const
{
	return m_startTime;
}

CString Nurse::GetClassification() const
{
	return m_classification;
}

void Nurse::SetClassification(const CString& str)
{
	m_classification = str;
}

int Nurse::GetPayPeriodHours() const
{
	return m_payPeriodHours;

}
void Nurse::SetPayPeriodHours(int period)
{
	m_payPeriodHours = period;
}


CString 
Nurse::IsWorkingOn( CTime date,ShiftLegend legend)const
{
	const FixedSchedule& entries = FixedSchedules();
	int size = entries.Size();
	for(int i=0;i<size;i++)
	{
		const FixedScheduleEntry& entry = entries[i];
		if (FixedSchedules().IsWorkingOn(date,i))
		{
			return 
			legend.GetAbbreviation(entry.
			StartTime(),entry.Hours());
		}
	}
	return _T("  ");
}

void operator<<(XMLNode& node, const Nurse& n)
{
	SerializeAttribute(node,_T("Name"),n.m_name);
	SerializeAttribute(node,_T("DailyHours"),n.m_dailyHours);
	SerializeAttribute(node,_T("WeeklyHours"),n.m_weeklyHours);
	Serialize(node,_T("FixedSchedule"),n.m_fixedSchedule);
	SerializeAttribute(node,_T("StartTime"),n.m_startTime);
	SerializeAttribute(node,_T("Classification"),n.m_classification,
						CString(_T("XXXX")));
	SerializeAttribute(node,_T("PayPeriodHours"),n.m_payPeriodHours,
						-1);
}
void operator>>(const XMLNode& node,Nurse&n)
{
	SerializeAttribute(node,_T("Name"),n.m_name);
	SerializeAttribute(node,_T("DailyHours"),n.m_dailyHours);
	SerializeAttribute(node,_T("WeeklyHours"),n.m_weeklyHours);
	Serialize(node,_T("FixedSchedule"),n.m_fixedSchedule);
	SerializeAttribute(node,_T("StartTime"),n.m_startTime);
	SerializeAttribute(node,_T("Classification"),n.m_classification,
						CString(_T("XXXX")));
	SerializeAttribute(node,_T("PayPeriodHours"),n.m_payPeriodHours,
						-1);
}

void Nurse::SetName(Name n)
{
	m_name = n;
}
Name Nurse::GetName() const
{
	return m_name;
}
void Nurse::SetDailyHours(int hours)
{
	m_dailyHours = hours;
}
int Nurse::GetDailyHours() const
{
	return m_dailyHours;
}
void Nurse::SetWeeklyHours(int hours)
{
	m_weeklyHours = hours;
}
int Nurse::GetWeeklyHours()const
{
	return m_weeklyHours;
}

void operator<<(XMLNode& node, const Nurses& n)
{
	Serialize(node,_T("NursesCollection"),n.m_nurses);

}
void operator>>(const XMLNode& node,Nurses& n)
{
	Serialize(node,_T("NursesCollection"),n.m_nurses);
	std::sort(n.m_nurses.begin(),n.m_nurses.end());
}