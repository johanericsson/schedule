#pragma once
#include <algorithm>

struct Empty
{
};

template <class T>
class Optional
{
public:
	Optional():
		m_t(NULL)
		{}
	Optional(Empty):
		m_t(NULL)
		{
		}
	Optional(const T& t)
	{
		m_t = new T(t);
	}

	operator void*() const {return m_t;}

	Optional(const Optional& o):
		m_t(NULL)
	{
		if (o.m_t)
			m_t = new T(*o.m_t);
	}

	void operator=(Optional o)
	{
		swap(o.m_t,m_t);
	}
	T* operator->() {return m_t;}
	const T* operator->() const {return m_t;}

	T& operator*() {return *m_t;}
	const T& operator*() const {return *m_t;}

	~Optional()
	{
		delete m_t;
	}
private:
	T * m_t;
};