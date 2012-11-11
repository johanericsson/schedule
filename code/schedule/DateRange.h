#pragma once
class XMLNode;
class DateRange
{
public:
	DateRange(){}
	DateRange(const CTime& startDate,int weeks);
	CTime GetStartDate()const;
	int GetNumberOfWeeks() const;
	CTime GetEndDate() const;
	CString ToString() const;
private:
	CTime m_startDate;
	int m_weeks;

	friend void operator<<(XMLNode& node,const DateRange&);	
	friend void operator>>(const XMLNode& node,DateRange&);	
};
