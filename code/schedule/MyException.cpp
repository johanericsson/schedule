#include "stdafx.h"
#include "MyException.h"

void Throw(const CString& str)
{
	throw OurException(str);
}
void Catch(const std::exception& e)
{
	AfxMessageBox(e.what());
}

const char * OurException::what()const
{
	return m_str;
}
void CheckWeekdayNumber(int i)
{
	if (i<0 || i>6)
	{
		CString str;
		str.Format(_T("%d is not a valid weekday index"),i);
		Throw(str);
	}
}