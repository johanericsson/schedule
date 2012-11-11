#pragma once
class Name
{
	CString m_first;
	CString m_last;
public:
	Name(){}
	Name(CString fullName);	
	Name(const CString& first,
		const CString& last):
	m_first(first),
	m_last(last)
	{
	}

	CString LastFirst() const
	{
		return m_last + _T(",") + m_first;
	}
	CString First() const {return m_first;}
	CString Last() const {return m_last;}
	
	CString FullName() const
	{
		return m_first + _T(" ") + m_last;
	}

	friend void operator<<(ostream& o, const Name& n);
	friend void operator>>(istream& i,Name& n);
};

inline bool IsEqual(const CString& fullName,const Name& name)
{
	return (fullName == name.FullName());
}

bool operator<(const Name& a,const Name& b);
inline bool operator==(const Name& a,const Name& b)
{
	if (!(a<b))
		if (!(b<a))
			return true;
	return false;
}





