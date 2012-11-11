#pragma once


template<class T>
class poly_ptr
{
public:
	poly_ptr(const T& t):
		m_p(t.Clone())
	{
	}

	T* operator->() 
	{
		return m_p;
	}
	const T* operator->() const
	{
		return m_p;
	}
	T& operator*()
	{
		return *m_p;
	}
	const T& operator*()const
	{
		return *m_p;
	}
	poly_ptr(const poly_ptr& p2):
	  m_p(p2.m_p->Clone())
	  {
	  }
	void operator=(poly_ptr p2)
	{
		swap(m_p,p2.m_p);
	}
	~poly_ptr()
	{
		delete m_p;
	}


	T* m_p;
};