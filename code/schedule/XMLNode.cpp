#include "stdafx.h"
#include "xmlnode.h"

XMLNode::XMLNode(const CString& title):
m_title(title)
{
}


const XMLNode& XMLNode::GetChild(const CString& title)const
{
	std::vector<const XMLNode*> children = GetChildren(title);
	if (children.size() == 0)
	{
		CString error = title;
		error += _T(" is not a child of ");
		error += m_title;
		Throw(error);
	}
	
	if (children.size() > 1)
	{
		CString error = title;
		error += _T(" is an unambigous child node of ");
		error += m_title;
	}
	return *children[0];
}
std::vector<const XMLNode*> 
XMLNode::GetChildren(const CString& title)const
{
	std::vector<const XMLNode*> children;
	for (std::vector<XMLNode>::const_iterator i = m_children.begin();
		i!=m_children.end();
		++i)
	{
		if (i->m_title == title)
		{
			children.push_back(&*i);
		}
	}
	return children;
}

struct TitleEquals
{
	CString m_title;
	bool operator()(const XMLAttribute& node)
	{
		if (m_title == _T("Name"))
			int i=0;
		return m_title == node.m_title;
	}
	TitleEquals(const CString& title):	
		m_title(title)
	{
	}
};


void XMLNode::GetAttribute(const CString& title, CString& value)const
{
	std::vector<XMLAttribute>::const_iterator i = 
		std::find_if(m_attributes.begin(),m_attributes.end(),TitleEquals(title));
	if (i==m_attributes.end())
	{
		CString error = _T("Could not find attribute:");
		error += title;
		error += _T(" inside the node:");
		error += m_title;
		Throw(error);
	}
	value = i->m_value;
}

void XMLNode::AddAttribute(const CString& title,const CString& value)
{
	std::vector<XMLAttribute>::const_iterator i = 
		std::find_if(m_attributes.begin(),m_attributes.end(),TitleEquals(title));
	if (i!= m_attributes.end())
	{
		CString error = _T("Can not add more than one attribute with the same name.");
		error += _T(" Tried to add: ");
		error += title;
		error += _T(" in node:");
		error += m_title;
	}
	XMLAttribute newAttribute;
	newAttribute.m_title = title;
	newAttribute.m_value = value;
	m_attributes.push_back(newAttribute);
}

void XMLNode::AddNode(const XMLNode& node)
{
	m_children.push_back(node);
}


void operator<<(ostream& o,const XMLNode& node)
{
	o << std::string("<");
	o << std::string(node.m_title);
	o << std::string(" ");
	for (int i =0;i<node.m_attributes.size();i++)
	{
		o << std::string(node.m_attributes[i].m_title);
		o << "=\"";
		o << std::string(node.m_attributes[i].m_value);
		o << "\" ";
	}
	if (node.m_children.empty())
	{
		o << std::string("/>");
		o << std::string("\n");
		return;
	}
	o << std::string(">");
	o << std::string("\n");
	for (int i=0;i<node.m_children.size();i++)
	{
		o << node.m_children[i];
	}
	o << std::string("</");
	o << std::string(node.m_title);
	o << std::string(">");
	o << std::string("\n");

}

static int lineNumber = 0;
CString GetLineNumber()
{
	CString line;
	line.Format(_T("%d"),lineNumber);
	return line;
}

struct MyStream
{
	const char * m_buffer;
	int m_current;
	int m_size;
	MyStream(const char* buffer,int size):
		m_buffer(buffer),
		m_size(size),
		m_current(NULL)
		{
		}
	char get()
	{
		if (m_current == m_size)
			Throw(_T("Took too much"));
		return m_buffer[m_current++];
	}
	void unget()
	{
		m_current--;
		if (m_current < 0)
			throw(_T("Gave back too much"));
	}
	operator const void*()
	{
		return this;
	}
	std::string getStr()
	{
		std::string str;
		while (1)
		{
			char c = get();
			if (c == ' ' || c == '\t' || c == '\n')
				return str;
			str.push_back(c);
		}
	}
};
void EatWhite(MyStream& i)
{
	char delin = ' ';
	for (;delin == ' ' || delin == '\t' || delin == '\n';)
	{
		if (delin == '\n')
			lineNumber++;
		if (lineNumber == 826)
			int aadf = 0;
		delin = i.get();
		if (!i)
			Throw (_T("Failure to read xml file"));
	}
	i.unget();
}

bool IsAlpha(char c)
{
	if ('a' <= c && c <= 'z')
		return true;
	if ('A' <= c && c <= 'Z')
		return true;
	return false;
}


void Read(MyStream& i, XMLNode& node)
{
	// first find the "<"
	char delin = 0;
	EatWhite(i);
	int z=0;
	for (;delin!='<';z++)
	{
		delin = i.get();
		if (!i)
			Throw(_T("Invalid XML Node. No '<'") + GetLineNumber());
	}
	// Then throw out all whitespace
	// next part is the title
	std::string title;
	title = i.getStr();
	if (!i)
		Throw(_T("Invalid XML Node. Bad title"));
	node.m_title = title.c_str();

	EatWhite(i);

	delin = i.get();
	if (!i)
		Throw(_T("Invalid XML node"));
	while (IsAlpha(delin))
	{
		// attribute
		std::vector<char> title;
		while (delin != '=')
		{
			title.push_back(delin);
			delin = i.get();
			if (!i)
				Throw(_T("Invalid XML Attribute"));
		}
		EatWhite(i);
		delin = i.get();
		if (!i)
			Throw(_T("No valid value for XML Attribute"));
		if (delin != '"')
			Throw(_T("Attributes need to be quoted"));
		delin = i.get();
		std::vector<char> value;
		while (delin!='"')
		{
			value.push_back(delin);
			delin = i.get();
			if (!i)
				Throw(_T("No valid value for XML Attribute"));
		}
		title.push_back(0);
		value.push_back(0);
		node.AddAttribute(CString(&title[0]),CString(&value[0]));
		EatWhite(i);
		delin = i.get();
	}

	if (delin == '/')
	{
		delin = i.get();
		if (delin != '>')
			Throw(_T("/ is only a valid character for ending a node"));
		return;
	}
	

	if (delin != '>') // end of node
		Throw(_T("Invalid character inside a node declaration"));
	
	int qq = 0;
	while (1)
	{
		qq++;
		if (qq == 55)
			int jj =0;
		EatWhite(i);
		// First check to see if we are closing off this node. Otherwise
		// process nodes recursively
		char firstChar = i.get();
		if (!i)
			Throw(_T("Node: ") + node.m_title + _T(" was never ended"));
		if (firstChar != '<')
			Throw(_T("Nodes can't have content"));
		delin = i.get();
		if (!i)
			Throw(_T("Invalid node"));
		if (delin == '/') // about to close the node
		{
			std::vector<char> title;
			delin = i.get(); // remove the '/'
			while (delin != '>')
			{
				title.push_back(delin);
				delin = i.get();
				if (!i)
					Throw(_T("Error in parsing closing node"));
			}
			title.push_back(0);
			if (node.m_title != &title[0])
				Throw(_T("Closing the wrong node. Expected: ") + node.m_title);
			return;
		}
		i.unget();
		ASSERT(i);
		i.unget();
		ASSERT(i);
		XMLNode newNode;
		Read(i,newNode);
		node.AddNode(newNode);
	}
}

void operator>>(istream& i,XMLNode& node)
{
	std::vector<char> buffer;
	while (i)
	{
		buffer.push_back(i.get());
	}
	MyStream str(&buffer[0],buffer.size());
	Read(str,node);
}