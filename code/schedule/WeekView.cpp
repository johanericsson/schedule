#include "stdafx.h"
#include "WeekView.h"
#include "period.h"
#include "Functions.h"
#include "Optional.h"
#include "schedule.h"
#include ".\weekview.h"


IMPLEMENT_DYNCREATE(WeekView,CScrollView)

BEGIN_MESSAGE_MAP(WeekView,CScrollView)
	ON_WM_CREATE()
	ON_COMMAND(ID_NEXT, OnNext)
	ON_COMMAND(ID_PREVIOUS, OnPrevious)
END_MESSAGE_MAP()


int WeekView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnInitialUpdate();

	return 0;
}

void WeekView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT,CSize(400,400));
	CScrollView::OnInitialUpdate();
}


void WeekView::Draw(const Period& period,
							int weekNumber,CDC* pDC)
{
	CString strWeekNumber;
	strWeekNumber.Format(_T("Week Number: %d"),m_weekNumber+1);
	CRect rcNames(40,10,40,10);
	CRect rcHeight(rcNames);
	pDC->DrawText(strWeekNumber,&rcHeight,DT_NOCLIP | DT_CALCRECT);
	int height = rcHeight.Height()+2;
	pDC->DrawText(strWeekNumber,rcNames,DT_NOCLIP);
	rcNames.OffsetRect(0,height);

	vector<vector<Slot> > cached(7);
	CString names(_T("Names"));
	pDC->DrawText(names,rcNames,DT_NOCLIP);
	


	CRect rcHoursNeeded(rcNames);
	rcHoursNeeded.OffsetRect(200,0);
	pDC->DrawText(_T("Hours Needed"),rcHoursNeeded,DT_NOCLIP);
	CRect rcHoursUsed(rcHoursNeeded);
	rcHoursUsed.OffsetRect(200,0);
	pDC->DrawText(_T("Hours Used"),rcHoursUsed,DT_NOCLIP);
	
	for(int i=0;i<period.m_data.m_nurses.Size();i++)
	{
		rcNames.OffsetRect(0,height);
		rcHoursNeeded.OffsetRect(0,height);
		rcHoursUsed.OffsetRect(0,height);

		const Nurse& nurse = period.m_data.m_nurses[i];
		

		int weeklyHours = nurse.GetWeeklyHours();
		CString strWeeklyHours;
		strWeeklyHours.Format(_T("%d\n"),weeklyHours);
		
		int numberOfHours = GetNumberOfHoursPerWeek(period,nurse,weekNumber,cached);
		CString strHoursUsed;
		strHoursUsed.Format(_T("%d\n"),numberOfHours);
		
		if (weeklyHours > 0)
		{
			if (weeklyHours < numberOfHours)
				pDC->SetTextColor(RGB(0,0,255));
			else
			if (weeklyHours > numberOfHours)
				pDC->SetTextColor(RGB(255,0,0));
		}

		pDC->DrawText(nurse.GetName().FullName(),rcNames,DT_NOCLIP);
		pDC->DrawText(strWeeklyHours,rcHoursNeeded,DT_NOCLIP);
		pDC->DrawText(strHoursUsed,rcHoursUsed,DT_NOCLIP);
		pDC->SetTextColor(RGB(0,0,0));
	}

	SetScrollSizes(MM_TEXT,CSize(rcHoursUsed.right,rcHoursUsed.bottom));
}

void WeekView::OnDraw(CDC * pDC)
{
	CFont courier;
	VERIFY(courier.CreatePointFont(120,_T("Courier New")));
	CFont * oldFont = pDC->SelectObject(&courier);
	
	CFont * newFont = pDC->SelectObject(oldFont);

	
	Draw(GetPeriod(),m_weekNumber,pDC);
	


	ASSERT(newFont == &courier);
}
void WeekView::OnNext()
{
	m_weekNumber++;
	if (m_weekNumber >= GetPeriod().m_dateRange.GetNumberOfWeeks())
		m_weekNumber--;
	Invalidate();
}

void WeekView::OnPrevious()
{
	m_weekNumber--;
	if (m_weekNumber< 0)
		m_weekNumber = 0;
	Invalidate();
}
