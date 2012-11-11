#include "stdafx.h"
#include "scheduleassignments.h"
#include "ordering.h"
#include "vectorhelper.h"

Assignment::Assignment(const Name& n,
			   int startTime,
			   int numberOfHours):
m_name(n),
m_startTime(startTime),
m_numberOfHours(numberOfHours)
{
}

void operator<<(XMLNode& n,const Assignment& a)
{
	SerializeAttribute(n,_T("Name"),a.m_name);
	SerializeAttribute(n,_T("NumberOfHours"),a.m_numberOfHours);
	SerializeAttribute(n,_T("StartTime"),a.m_startTime);
}
void operator>>(const XMLNode& n,Assignment& a)
{
	SerializeAttribute(n,_T("Name"),a.m_name);
	SerializeAttribute(n,_T("NumberOfHours"),a.m_numberOfHours);
	SerializeAttribute(n,_T("StartTime"),a.m_startTime);
}
	
Name Assignment::GetName() const
{
	return m_name;
}

int Assignment::StartTime() const
{
	return m_startTime;
}

int Assignment::NumberOfHours()const
{
	return m_numberOfHours;
}
bool operator<(const Assignment& a1,
						  const Assignment& a2)
{
	COMPARE(m_startTime)
	COMPARE(m_numberOfHours)
	COMPARE(m_name)
	return false;
}

void operator<<(XMLNode& n,const FixedScheduleOverride& a)
{
	SerializeAttribute(n,_T("Nurse"),a.m_nurse);
}
void operator>>(const XMLNode& n,FixedScheduleOverride& a)
{
	SerializeAttribute(n,_T("Nurse"),a.m_nurse);
}

void DailyOverrides::Add(const Assignment& assignment)
{
	// if a person already has an assignment, then they can't have
	// another assignment for this day.
	for (int i=0;i<m_assignments.size();i++)
	{
		if (m_assignments[i].GetName() == assignment.GetName())
			Throw(assignment.GetName().FullName() + _T(" already has an assignment for this day."));
	}
	m_assignments.push_back(assignment);
	sort(m_assignments.begin(),m_assignments.end());
}
void DailyOverrides::AddOverride(const FixedScheduleOverride& override)
{
	// First check whether it already exists:
	for (int i=0;i<m_overrides.size();i++)
	{
		if (override == m_overrides[i])
			return;
	}
	m_overrides.push_back(override);
}

void operator<<(XMLNode& n,const TemporarySchedule& t)
{
	SerializeAttribute(n,_T("NumberOfSlots"),t.numberOfSlots);
	SerializeAttribute(n,_T("StartTime"),t.startTime);
}
void operator>>(const XMLNode& n,TemporarySchedule& t)
{
	SerializeAttribute(n,_T("NumberOfSlots"),t.numberOfSlots);
	SerializeAttribute(n,_T("StartTime"),t.startTime);
}

void operator<<(XMLNode& n,const VacationSchedule& s)
{
	SerializeAttribute(n,_T("Name"),s.m_nurse);
	SerializeAttribute(n,_T("Code"),s.m_code);

}
void operator>>(const XMLNode& n,VacationSchedule& s)
{
	SerializeAttribute(n,_T("Name"),s.m_nurse);
	SerializeAttribute(n,_T("Code"),s.m_code);
}

void operator<<(XMLNode& n,const DailyOverrides& a)
{
	Serialize(n,_T("Assignments"),a.m_assignments);
	Serialize(n,_T("Overrides"),a.m_overrides);
	Serialize(n,_T("TemporarySchedule"),a.m_temporarySchedule);
	Serialize(n,_T("Vacations"),a.m_vacationSchedules,vector<VacationSchedule>());

}
void operator>>(const XMLNode& n,DailyOverrides& a)
{
	Serialize(n,_T("Assignments"),a.m_assignments);
	Serialize(n,_T("Overrides"),a.m_overrides);
	Serialize(n,_T("TemporarySchedule"),a.m_temporarySchedule);
	Serialize(n,_T("Vacations"),a.m_vacationSchedules,vector<VacationSchedule>());
}

void operator<<(XMLNode& n,const ScheduleAssignments& a)
{
	Serialize(n,_T("DailyAssignments"),a.m_daily);
}
void operator>>(const XMLNode& n,ScheduleAssignments& a)
{
	Serialize(n,_T("DailyAssignments"),a.m_daily);
}