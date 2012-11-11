#pragma once
#include "XMLNode.h"

template <class T> 
void operator<<(XMLNode& node, const std::vector<T>& v)
{
	for (int i=0;i<v.size();i++)
	{
		XMLNode newNode(_T("CollectionElement"));
		newNode.AddAttribute(_T("ElementNumber"),i);
		newNode << v[i];
		node.AddNode(newNode);
	}
}

template <class T>
void operator>>(const XMLNode& node,std::vector<T>& v)
{
	std::vector<const XMLNode*> collection = 
		node.GetChildren(_T("CollectionElement"));
	v.resize(collection.size());
	for (int i=0;i<collection.size();i++)
	{
		(*collection[i]) >> v[i];
	}
}

template <class T>
class sorted_vector
{
public:
	sorted_vector()
	{
	}
	sorted_vector(const T& t,int size):
		m_vector(t,size)
		{
			sort();
		}

	T& operator[](int i)
	{
		return m_vector.at(i);
	}
	const T& operator[](int i) const
	{
		return m_vector.at(i);
	}
	void push_back(const T& t)
	{
		m_vector.push_back(t);
		sort();
	}
	std::vector<T> Get() const
	{
		return m_vector;
	}
	long size() const
	{
		return m_vector.size();
	}
	typename std::vector<T>::iterator begin() 
	{
		return m_vector.begin();
	}
	typename std::vector<T>::const_iterator begin()const 
	{
		return m_vector.begin();
	}
	typename std::vector<T>::iterator end() 
	{
		return m_vector.end();
	}
	typename std::vector<T>::const_iterator end()const 
	{
		return m_vector.end();
	}
	void Add(const vector<T>& in)
	{
		for (int i=0;i<in.size();i++)
		{
			m_vector.push_back(in[i]);
		}
		sort();
	}
	void erase(typename std::vector<T>::iterator i)
	{
		m_vector.erase(i);
	}
	void sort()
	{
		std::sort(m_vector.begin(),m_vector.end());
	}
	T& back()
	{
		return m_vector.back();
	}
	const T& back() const
	{
		return m_vector.back();
	}
private:
	std::vector<T> m_vector;
};