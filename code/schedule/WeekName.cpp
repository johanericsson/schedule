#include "stdafx.h"
#include "WeekNames.h"
#include "assert.h"
#include "MyException.h"


static const char * weekNames[] = 
{
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};


CString WeekName(int i)
{
	CheckWeekdayNumber(i);
	return weekNames[i];
}