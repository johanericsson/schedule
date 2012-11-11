#pragma once
struct ShiftTemplate
{
	ShiftTemplate(int time,int nurses,int hours):startTime(time),nurses(nurses),hours(hours){}
	int startTime;
	int hours;
	int nurses;
	ShiftTemplate():startTime(-1),nurses(-1){}
};

class XMLNode;
void operator<<(XMLNode& node,const ShiftTemplate&);	
void operator>>(const XMLNode& node,ShiftTemplate&);

CString ToString(ShiftTemplate shift);

class WeekDayTemplate
{
	int m_weekDay;
	vector<ShiftTemplate> m_shifts;
public:
	CString GetWeekName() const;

	void AddTemplate(int startTime,
			  int nurses,
			  int hours);

	WeekDayTemplate(){}
	WeekDayTemplate(int day);

	int Size() const {return m_shifts.size();}
	// 3, 7, 11, 15, 19, 23
	const ShiftTemplate& operator[](int i) const {return m_shifts[i];}
	ShiftTemplate& operator[](int i) {return m_shifts[i];}
	void Remove(int i) {m_shifts.erase(m_shifts.begin() + i);}

	friend void operator<<(XMLNode& ar,const WeekDayTemplate& t);
	friend void operator>>(const XMLNode& ar,WeekDayTemplate& t);

};

struct ShiftsTemplate
{
	ShiftsTemplate();
	const WeekDayTemplate& operator[](int weekDay) const
	{
		return m_Days[weekDay];
	}
	WeekDayTemplate& operator[](int weekDay)
	{
		return m_Days[weekDay];
	}
private:
	vector<WeekDayTemplate> m_Days;

	friend void operator<<(XMLNode& node,const ShiftsTemplate&);	
	friend void operator>>(const XMLNode& node,ShiftsTemplate&);	
};