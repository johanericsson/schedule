#include "stdafx.h"
#include "DailyOverview.h"
#include "functions.h"
#include "period.h"
#include "schedule.h"
#include ".\dailyoverview.h"


IMPLEMENT_DYNCREATE(DailyOverview,CScrollView)

BEGIN_MESSAGE_MAP(DailyOverview,CScrollView)
ON_WM_CREATE()
ON_COMMAND(ID_NEXT, OnNextDay)
ON_COMMAND(ID_PREVIOUS, OnPreviousDay)
ON_COMMAND(ID_FAST_FORWARD, OnNextWeek)
ON_COMMAND(ID_FAST_REVERSE, OnPreviousWeek)
END_MESSAGE_MAP()



int DailyOverview::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetScrollSizes(MM_TEXT,CSize(400,400));

	return 0;
}

int DailyOverview::m_dayInitializer = 0;
struct NumberWorkingAtHour
{
	int hour;
	int number;
	NumberWorkingAtHour(int hour,int number):
		hour(hour),
		number(number)
		{
		}
};
void DailyOverview::OnDraw(CDC * pDC)
{
	CTime day = GetPeriod().m_dateRange.GetStartDate() + CTimeSpan(m_day,0,0,0);
	vector<Slot> working = GetWorkingNurses(GetPeriod(),day);
	vector<NumberWorkingAtHour> workingAtHour;
	for (int i=0;i<24;i++)
	{
		workingAtHour.push_back(NumberWorkingAtHour(i,0));
		for(int j=0;j<working.size();j++)
		{
			int startTime = working[j].startTime;
			int endTime = startTime + working[j].hours;
			if (startTime <= i &&
				i < endTime)
				workingAtHour[i].number++;
		}
	}
	// collapse like minded
	NumberWorkingAtHour last(workingAtHour[0]);
	for (int i=0;i<workingAtHour.size();)
	{
		if (workingAtHour[i].number == last.number)
			workingAtHour.erase(workingAtHour.begin() + i);
		else
		{
			last = workingAtHour[i];
			i++;
		}
	}
	CString strOut = day.Format(_T("%A, %B %d, %Y\n"));
	for (int i=0;i<workingAtHour.size();i++)
	{
		CString line;
		line.Format(_T("At hour:%d. Working: %d\n"),workingAtHour[i].hour,
						workingAtHour[i].number);
		strOut += line;
	}

	CRect rcOut(40,40,40,40);
	CRect rcCalc(rcOut);
	pDC->DrawText(strOut,&rcCalc,DT_NOCLIP | DT_CALCRECT);
	pDC->DrawText(strOut,rcOut,DT_NOCLIP);

	SetScrollSizes(MM_TEXT,CSize(rcCalc.right,rcCalc.bottom));

}

static int GetMaxDays()
{
	DateRange& dateRange = GetPeriod().m_dateRange;
	CTime start = dateRange.GetStartDate();
	CTime stop = dateRange.GetEndDate();
	return (int)(stop-start).GetDays();
}
void DailyOverview::OnNextDay()
{
	int max = GetMaxDays();
	if (m_day + 1 != max)
		m_day++;
	Invalidate();
}

void DailyOverview::OnPreviousDay()
{
	if (m_day)
		m_day--;
	Invalidate();
}

void DailyOverview::OnNextWeek()
{
	int max = GetMaxDays();
	if (m_day + 7 >= max)
		m_day = max-1;
	else
		m_day += 7;
	Invalidate();
}

void DailyOverview::OnPreviousWeek()
{
	if (m_day - 7 < 0)
		m_day = 0;
	else
		m_day -= 7;
	Invalidate();
}