#pragma once
#include "Name.h"
#include "FixedSchedule.H"
class ShiftLegend;
class XMLNode;
class Nurse
{
public:
	Nurse() {}
	void SetName(Name n);
	Name GetName() const;
	void SetDailyHours(int hours);
	int GetDailyHours() const;
	void SetWeeklyHours(int hours);
	int GetWeeklyHours()const;
	int GetStartTime()const;
	void SetStartTime(int startTime);
	Nurse(Name,int dailyHours =8,int weeklyHours=32,
			int startTime = -1);
	FixedSchedule& FixedSchedules()
	{
		return m_fixedSchedule;
	}
	const FixedSchedule& FixedSchedules()const
	{
		return m_fixedSchedule;
	}
	CString IsWorkingOn(CTime date,
						ShiftLegend legend)const;

	CString GetClassification() const;
	void SetClassification(const CString& str);

	int GetPayPeriodHours() const;
	void SetPayPeriodHours(int period);
private:
	FixedSchedule m_fixedSchedule;
	Name m_name;
	CString m_classification;
	int m_payPeriodHours;
	int m_dailyHours;
	int m_periodHours;
	int m_weeklyHours;
	int m_startTime;

	friend void operator<<(XMLNode& ar, const Nurse&);
	friend void operator>>(const XMLNode& ar,Nurse&n);
	friend bool operator<(Nurse,Nurse);
};

inline bool operator<(Nurse a,Nurse b)
{
	return (a.m_name < b.m_name);
}

class Nurses
{
public:
	vector<Nurse> m_nurses;
	Nurse& operator[](int i) {return m_nurses[i];}
	
	const Nurse& operator[](int i) const {return m_nurses[i];}
	void Add(Nurse n)
	{
		m_nurses.push_back(n);
		sort(m_nurses.begin(),m_nurses.end());
	}
	void Resort()
	{
		sort(m_nurses.begin(),m_nurses.end());
	}
	void Remove(int i)
	{
		m_nurses.erase(m_nurses.begin() + i);
	}
	vector<Nurse>::const_iterator begin() const {return m_nurses.begin();}
	vector<Nurse>::const_iterator end() const {return m_nurses.end();}
	int Size() const {return m_nurses.size();}

	friend void operator<<(XMLNode& ar, const Nurses&);
	friend void operator>>(const XMLNode& ar,Nurses&n);
};


