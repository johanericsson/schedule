#pragma once

#include <exception>
struct OurException : exception
{
	OurException(const CString& str): m_str(str){}
	CString m_str;

	virtual const char * what()const;
};
void CheckWeekdayNumber(int i);
void Throw(const CString& str);
void Catch(const std::exception& e);
#define MY_CATCH() catch(const std::exception& e) {Catch(e);}