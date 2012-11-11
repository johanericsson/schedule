#pragma once
class XMLNode;
struct Abbreviation
{
	Abbreviation():
		startTime(-1),
			hours(0){}
	
	Abbreviation(int startTime,int hours,CString abbreviation):
		startTime(startTime),
		hours(hours),
		abbreviation(abbreviation)
		{
		}

	int startTime;
	int hours;
	CString abbreviation;
};
void operator<<(XMLNode&,const Abbreviation&);
void operator>>(const XMLNode&,Abbreviation&);

bool operator<(Abbreviation,Abbreviation);
CString ToString(Abbreviation);

class ShiftLegend
{
public:
	ShiftLegend(){}

	void AddAbbreviation(Abbreviation);
	int Size() const;
	void Remove(int i);
	Abbreviation& operator[](int i);
	const Abbreviation& operator[](int i) const;
	CString GetAbbreviation(int startTime,int hours)const;
private:

	friend void operator<<(XMLNode&,const ShiftLegend&);
	friend void operator>>(const XMLNode&,ShiftLegend&);
	vector<Abbreviation> m_abbreviations;
};