#include "stdafx.h"
#include "Shifts.h"
#include <algorithm>
#include "vectorhelper.h"
#include "weeknames.h"
#include "ordering.h"
CString ToString(ShiftTemplate shift)
{
	CString form;
	form.Format("Start:%d Hours:%d Employees:%d",shift.startTime,shift.hours,shift.nurses);
	return form;
}

void operator<<(XMLNode& node,const ShiftTemplate& t)
{
	node.AddAttribute(_T("StartTime"),t.startTime);
	node.AddAttribute(_T("Nurses"),t.nurses);
	node.AddAttribute(_T("Hours"),t.hours);
}
void operator>>(const XMLNode& node,ShiftTemplate& t)
{
	node.GetAttribute(_T("StartTime"),t.startTime);
	node.GetAttribute(_T("Nurses"),t.nurses);
	node.GetAttribute(_T("Hours"),t.hours,12);
}

bool Compare(ShiftTemplate a1,ShiftTemplate a2)
{
	COMPARE(startTime);
	COMPARE(hours);
	COMPARE(nurses);
	return false;
}

void WeekDayTemplate::AddTemplate(
			  int startTime,
			  int nurses,
			  int hours)

{	
	m_shifts.push_back(ShiftTemplate(startTime,nurses,hours));
	sort(m_shifts.begin(),m_shifts.end(),&Compare);
}

CString WeekDayTemplate::GetWeekName()const
{
	return WeekName(m_weekDay);

}

WeekDayTemplate::WeekDayTemplate(int weekDay):
m_weekDay(weekDay)
{
	if (weekDay ==0) // sunday
	{
		//AddShift(m_Shifts,3,1);
		//AddShift(m_Shifts,19,2);
		//AddShift(m_Shifts,23,4);
	}
	else if (weekDay %2)
	{
		AddTemplate(7,2,12);
		AddTemplate(15,3,12);
		AddTemplate(23,2,12);
	}
	else
	{
		AddTemplate(3,2,12);
		AddTemplate(7,3,12);
		AddTemplate(11,2,12);
		AddTemplate(15,2,12);
		AddTemplate(19,3,12);
		AddTemplate(23,2,12);
	}
}

void operator<<(XMLNode& node,const WeekDayTemplate& st)
{
	node.AddAttribute(_T("WeekDay"),st.m_weekDay);
	Serialize(node,_T("Shifts"),st.m_shifts);
}
void operator>>(const XMLNode& node,WeekDayTemplate& st)
{
	node.GetAttribute(_T("WeekDay"),st.m_weekDay);
	Serialize(node,_T("Shifts"),st.m_shifts);
}


ShiftsTemplate::ShiftsTemplate()
{
	for (int i=0;i<7;i++)
	{
		m_Days.push_back(WeekDayTemplate(i));
	}

}

void operator<<(XMLNode& node,const ShiftsTemplate& st)
{
	Serialize(node,CString(_T("Days")),st.m_Days);
}
void operator>>(const XMLNode& node,ShiftsTemplate& st)
{
	Serialize(node,CString(_T("Days")),st.m_Days);
}