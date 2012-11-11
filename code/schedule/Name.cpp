#include "stdafx.h"
#include "name.h"
#include "MyException.h"
#include "VectorHelper.h"
#include "Ordering.h"
inline void RemoveLeadingSpaces(vector<char>& buf)
{
	while (buf.size() != 0 && buf.front() == ' ')
	{
		buf.erase(buf.begin());
	}
}

bool IsSeperator(char a)
{
	if (a == ' ' ||
		a == ',')
		return true;
	return false;
}

Name::Name(CString fullName)
{
	if (fullName == "  ")
	{
		this->m_first = " ";
		this->m_last = " ";
		return;
	}


	const char * front = fullName.GetBuffer(0);
	const char * end = fullName.GetBuffer(0) + fullName.GetLength();
	vector<char> buf(front,end);

	// first find the ,
	RemoveLeadingSpaces(buf);
	vector<char>::iterator i = find_if(buf.begin(),buf.end(),IsSeperator);
	if (i== buf.end())
		Throw("Names must include a space");

	vector<char> first(buf.begin(), i);

	while (i < buf.end() &&
		   *i == ' ')
	{
		++i;
	}
	if (i== buf.end())
		Throw("Names must have two words");

	bool Flipped = (*i == ',');

	if (Flipped)
	{
		++i;
		while (i < buf.end() && 
			   *i == ' ')
		{
			++i;
		}
	}
	if (i==buf.end())
		Throw("Names must have two words");

	vector<char> second(i,buf.end());
	while (second.back() == ' ')
		second.erase(second.end() - 1);
	first.push_back(0);
	second.push_back(0);
	if (Flipped)
	{ 
		* this = Name(&second[0],&first[0]);
	}
	else
		*this = Name(&first[0],&second[0]);

}

bool operator<(const Name& a1,const Name& a2)
{
	COMPARE(Last())
	COMPARE(First())
	return false;
}

void operator<<(ostream& o, const Name& n)
{
	std::string str(n.m_first);
	o << str;
	if (!o)
		return;
	o << " ";
	if (!o)
		return;
	str = std::string(n.m_last); 
	o << str;
}
void operator>>(istream& i,Name& n)
{
	std::string str;
	i >> str;
	if (!i)
		return;
	n.m_first = str.c_str();
	i >> str;
	if (!i)
		return;
	n.m_last = str.c_str();
}
