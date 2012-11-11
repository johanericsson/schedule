#pragma once 
#include "myexception.h"
#include <sstream>

struct XMLAttribute
{
	CString m_title;
	CString m_value;
};

void operator<<(ostream& o,const XMLAttribute& attribute);
void operator>>(istream& i,XMLAttribute& attribute);

struct MyStream;
class XMLNode
{
private:
	CString m_title;
	std::vector<XMLNode> m_children;
	std::vector<XMLAttribute> m_attributes;
public:
	void SetTitle(const CString& title)
	{
		m_title = title;
	}
	CString GetTitle() const
	{
		return m_title;
	}
	XMLNode(){}
	XMLNode(const CString& title);
	std::vector<const XMLNode*> GetChildren(const CString& title)const;
	const XMLNode& GetChild(const CString& title)const;

	void GetAttribute(const CString& title, CString& value)const;

	void AddAttribute(const CString& title,const CString& value);
	void AddNode(const XMLNode& node);

	template <class T>
	void GetAttribute(const CString& title,T& t)const
	{
		CString strValue;
		GetAttribute(title,strValue);
		std::string str(strValue);
		std::istringstream stream(str);
		stream >> t;
		if (!stream)
		{
			CString failureMessage = _T("Failure during the reading of attribute:");
			failureMessage += title;
			failureMessage += _T(" in node:");
			failureMessage += m_title;
			Throw(failureMessage);
		}
	}

	template <class T>
	void GetAttribute(const CString& title, T& t,const T& def)const
	{
		try
		{
			GetAttribute(title,t);
		}
		catch(const OurException&)
		{
			// use the default type if we can't load the type
			t = def;
		}
	}
	template <class T>
	void AddAttribute(const CString& title, const T& t)
	{
		std::ostringstream str;
		str << t;
		if (!str)
		{
			CString failureMessage = _T("Failure during the writing of attribute:");
			failureMessage += title;
			failureMessage += _T(" in node:");
			failureMessage += m_title;
			Throw(failureMessage);
		}
		AddAttribute(title,CString(str.str().c_str()));
	}
	friend void operator<<(ostream& o,const XMLNode& node);
	friend void operator>>(istream& i,XMLNode& node);
	friend void Read(MyStream& i, XMLNode& node);
};

template <class T>
inline void Serialize(const XMLNode& source,const CString& title,T& destination)
{
	source.GetChild(title) >> destination;
}

template <class T>
inline void Serialize(const XMLNode& source,
					  const CString& title,
					  T& destination,
					  const T& def)
{
	try
	{
		source.GetChild(title) >> destination;
	}
	catch(const OurException&)
	{
		destination = def;
	}
}

template <class T>
inline void Serialize(XMLNode& destination,const CString& title,const T& source)
{
	XMLNode newNode(title);
	newNode << source;
	destination.AddNode(newNode);
}

template <class T>
inline void Serialize(XMLNode& destination,
					  const CString& title,
					  const T& source,
					  const T&)
{
	XMLNode newNode(title);
	newNode << source;
	destination.AddNode(newNode);
}


template <class T>
inline void SerializeAttribute(const XMLNode& source,
							   const CString& title,T& destination)
{
	source.GetAttribute(title,destination);
}

template <class T>
inline void SerializeAttribute(const XMLNode& source,
							   const CString& title,T& destination,
							   const T& def)
{
	source.GetAttribute(title,destination,def);
}

template <class T>
inline void SerializeAttribute(XMLNode& destination,
							   const CString& title,const T& source,
							   const T&)
{
	destination.AddAttribute(title,source);
}

template <class T>
inline void SerializeAttribute(XMLNode& destination,
							   const CString& title,const T& source)
{
	destination.AddAttribute(title,source);
}

void operator<<(ostream& o,const XMLNode& node);
void operator>>(istream& i,XMLNode& node);