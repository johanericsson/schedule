#pragma once
class XMLNode;
class FixedScheduleEntry
{
public:
	FixedScheduleEntry() {}
	FixedScheduleEntry(int week,int day,int startTime,int Hours);
	CString ToString() const;
	int Week() const;
	int Day() const;
	int StartTime() const;
	int Hours() const;
private:
	int m_week;
	int m_day;
	int m_startTime;
	int m_hours;
	friend void operator<<(XMLNode& ar,const FixedScheduleEntry& f);
	friend void operator>>(const XMLNode& ar,FixedScheduleEntry& f);
};

bool operator<(const FixedScheduleEntry& f1,
			   const FixedScheduleEntry& f2);

class FixedSchedule
{
public:
	FixedSchedule();

	void Add(FixedScheduleEntry entry);
	bool IsOnlyAvailable()const
	{
		return m_bOnlyAvailable;
	}

	void SetOnlyAvailable(bool bTrue)
	{
		m_bOnlyAvailable = bTrue;
	}
	void SetNumberOfWeeks(int i);
	int GetNumberOfWeeks() const;
	void SetOffset(int weeks);
	int GetOffset() const;
	FixedScheduleEntry& operator[](int i);
	const FixedScheduleEntry& operator[](int i)const;
	int Size() const;
	void Remove(int i);

	friend void operator<<(XMLNode& ar,const FixedSchedule& f);
	friend void operator>>(const XMLNode& ar,FixedSchedule& f);
	bool IsWorkingOn(CTime day,int entry)const;
	
private:
	bool m_bOnlyAvailable;
	int m_offset;
	int m_numberOfWeeks;
	vector<FixedScheduleEntry> m_fixedScheduleEntries;
};
