#pragma once
#include "Name.h"
#include "fixedSchedule.h"
#include "ordering.h"

class Assignment
{
public:
	Assignment(const Name& n,
			   int startTime,
			   int numberOfHours);
	Assignment(){}
	Name GetName() const;
	int StartTime() const;
	int NumberOfHours()const;
	CString ToString() const
	{
		CString startTime;
		startTime.Format(_T("%d:%d:"),
							m_startTime,NumberOfHours());
		return startTime + GetName().FullName();
	}

	friend void operator<<(XMLNode& n,const Assignment& a);
	friend void operator>>(const XMLNode& n,Assignment& a);
private:
	int m_startTime;
	int m_numberOfHours;
	Name m_name;

	friend bool operator<(const Assignment& a1,
						  const Assignment& a2);
};

class FixedScheduleOverride
{
public:
	FixedScheduleOverride(const Name& name):
	 m_nurse(name)
	 {
	 }
	 FixedScheduleOverride() {}
	Name m_nurse;
	bool operator==(const FixedScheduleOverride&
		o)const
	{
		return (m_nurse == o.m_nurse);
	}

	friend void operator<<(XMLNode& n,const FixedScheduleOverride& a);
	friend void operator>>(const XMLNode& n,FixedScheduleOverride& a);
};

class TemporarySchedule
{
public:
	TemporarySchedule()
	{
	}
	TemporarySchedule(int startTime,int numberOfSlots):
		startTime(startTime),
		numberOfSlots(numberOfSlots)
		{
		}
	int startTime;
	int numberOfSlots;
	friend bool operator<(TemporarySchedule a1,TemporarySchedule a2)
	{
		COMPARE(startTime);
		COMPARE(numberOfSlots);
		return true;
	}
	friend bool operator==(TemporarySchedule a1,TemporarySchedule a2)
	{
		return (a1.startTime == a2.startTime);
	}

};

void operator<<(XMLNode& n,const TemporarySchedule& sched);
void operator>>(const XMLNode& n,TemporarySchedule& sched);

struct VacationSchedule
{
	Name m_nurse;
	CString m_code;
};

void operator<<(XMLNode& n,const VacationSchedule& sched);
void operator>>(const XMLNode& n,VacationSchedule& sched);

class DailyOverrides
{
public:
	int Size() const
	{
		return m_assignments.size();
	}
	void Remove(int i)
	{
		m_assignments.erase(m_assignments.begin() + i);
	}
	void Add(const Assignment& assignment);
	Assignment& operator[](int i)
	{
		return m_assignments[i];
	}
	const Assignment& operator[](int i)const
	{
		return m_assignments[i];
	}
	void AddOverride(const FixedScheduleOverride& override);
	vector<FixedScheduleOverride> m_overrides;
	vector<TemporarySchedule> m_temporarySchedule;
	vector<VacationSchedule> m_vacationSchedules;
private:
	vector<Assignment> m_assignments;

	friend void operator<<(XMLNode& ar,const DailyOverrides& assignments);
	friend void operator>>(const XMLNode& ar,DailyOverrides& assignments);
};



class ScheduleAssignments
{
public:
	ScheduleAssignments(){}
	DailyOverrides& operator[](int i)
	{
		if (i>= m_daily.size())
			m_daily.resize(i+1);
		return m_daily.at(i);
	}
	const DailyOverrides& operator[](int i)const
	{
		if (i>=m_daily.size())
			(const_cast<vector<DailyOverrides>&>(m_daily)).resize(i+1);
		return m_daily.at(i);
	}

	friend void operator<<(XMLNode& ar,const ScheduleAssignments& assignments);
	friend void operator>>(const XMLNode& ar,ScheduleAssignments& assignments);
	vector<DailyOverrides> m_daily;
private:
};

