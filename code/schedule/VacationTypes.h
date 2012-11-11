#pragma once
class XMLNode;
struct VacationType
{
	CString m_code;
	CString m_description;
};

inline bool operator<(const VacationType& t1,const VacationType& t2)
{
	return t1.m_code < t2.m_code;
}

CString ToString(const VacationType& t);

void operator<<(XMLNode& node,const VacationType&);
void operator>>(const XMLNode& node,VacationType&);

struct VacationTypes
{
	const VacationType& operator[](int i)const
	{
		return m_types[i];
	}
	VacationType& operator[](int i)
	{
		return m_types[i];
	}
	int Size() const 
	{
		return m_types.size();
	}
	vector<VacationType> m_types;
};


void operator<<(XMLNode& node,const VacationTypes&);
void operator>>(const XMLNode& node,VacationTypes&);