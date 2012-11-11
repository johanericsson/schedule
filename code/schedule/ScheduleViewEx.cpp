#include "stdafx.h"
#include "scheduleViewEx.h"
#include "period.h"
#include "scheduleview.H"
#include "schedule.h"
static const int width = 11000;
static const int height = 8500;
static const int printFontSize = 1100;
static const int viewFontSize = 1500;

int GetLongestName(const Period& period)
{
	const Nurses& nurses = period.m_data.m_nurses;
	int length = 0;
	for (int i=0;i<nurses.Size();i++)
	{
		length = max(length,nurses[i].GetName().FullName().GetLength());
	}
	return length;
}





IMPLEMENT_DYNCREATE(ScheduleViewEx,CScrollView)
BEGIN_MESSAGE_MAP(ScheduleViewEx,CScrollView)
	ON_WM_CREATE()
	ON_COMMAND(ID_PREVIOUS, OnPrevious)
	ON_COMMAND(ID_NEXT, OnNext)
	ON_COMMAND(ID_PRINTALL, OnPrintAll)
END_MESSAGE_MAP()


BOOL ScheduleViewEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;


	SetScrollSizes(MM_HIENGLISH,CSize(width,height));
	GetParent()->SetWindowText(_T("ScheduleView"));

	return TRUE;

}

template<class T>
static void Truncate(T& value,T min,T max)
{
	if (value > max)
		value = max;
	if (value < min)
		value = min;
}

void ScheduleViewEx::UpdateTheSubject()
{
	Truncate(m_offset,0,TotalPages()-1);
	Invalidate();
}




static const int xDelta = 7*4;
static const int yDelta = 42;
void ScheduleViewEx::OnDraw(CDC* pDC)
{
	Periods& periods = static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	Period& period = periods.GetCurrentPeriod();

	CTime startDay = period.m_dateRange.GetStartDate();
	startDay = startDay + CTimeSpan(xDelta*GetXOffset(),0,0,0);
	CTime stopDay = startDay + CTimeSpan(xDelta,0,0,0);

	Truncate(startDay,period.m_dateRange.GetStartDate(),
		period.m_dateRange.GetEndDate());
	Truncate(stopDay,period.m_dateRange.GetStartDate(),
		period.m_dateRange.GetEndDate());
	
	
	int startNurse = yDelta * GetYOffset();
	int stopNurse = startNurse + yDelta;
	
	CString outputText = ViewOutput(startDay,stopDay,
		startNurse,stopNurse);

	CFont tmpcourierFont;
	if (pDC->m_bPrinting)
		tmpcourierFont.CreatePointFont(printFontSize,_T("Lucida Console"));
	else
		tmpcourierFont.CreatePointFont(viewFontSize,_T("Lucida Console"));
	LOGFONT logFont;
	tmpcourierFont.GetLogFont(&logFont);
	logFont.lfUnderline = 1;
	
	CFont courierFont;
	courierFont.CreateFontIndirect(&logFont);
	
	CFont * oldFont;
	oldFont = pDC->SelectObject(&courierFont);

	pDC->DrawText(outputText,CRect(250,-250,250,-250),DT_NOCLIP);
	if (!pDC->m_bPrinting)
	{
		CRect rcOut(0,0,0,0);
		VERIFY(pDC->DrawText(outputText,&rcOut,DT_NOCLIP | DT_CALCRECT));
		CSize newSize(rcOut.right*1.1,(rcOut.bottom*-1)*1.1);
		if (newSize != m_oldSize)
		{
			SetScrollSizes(MM_HIENGLISH,newSize);
			m_oldSize = newSize;
		}
	}

	CFont * tmpFont = pDC->SelectObject(oldFont);
	ASSERT(tmpFont = &courierFont);

}

void ScheduleViewEx::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
}



int ScheduleViewEx::GetXOffset() const
{
	Periods& periods = static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	Period& period = periods.GetCurrentPeriod();
	CTimeSpan span = period.m_dateRange.GetEndDate() - 
		period.m_dateRange.GetStartDate();
	int numberOfXPages = (int)(span.GetDays() / xDelta);
	if (span.GetDays() % xDelta) // if a partial page, then add 1
		numberOfXPages++;
	int xOffset = m_offset  % numberOfXPages;
	return xOffset;
}

int ScheduleViewEx::GetYOffset() const
{
	Periods& periods = static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	Period& period = periods.GetCurrentPeriod();
	CTimeSpan span = period.m_dateRange.GetEndDate() - 
		period.m_dateRange.GetStartDate();
	
	int numberOfXPages = (int)(span.GetDays() / xDelta);
	if (span.GetDays() % xDelta) // if a partial page, then add 1
		numberOfXPages++;

	int yOffset = m_offset / numberOfXPages;
	return yOffset;
}

int ScheduleViewEx::TotalPages() const
{
	Periods& periods = static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	Period& period = periods.GetCurrentPeriod();
	CTimeSpan span = period.m_dateRange.GetEndDate() - 
		period.m_dateRange.GetStartDate();
	
	int numberOfXPages = (int)(span.GetDays() / xDelta);
	if (span.GetDays() % xDelta) // if a partial page, then add 1
		numberOfXPages++;
	int numberOfYPages = period.m_data.m_nurses.Size() / yDelta;
	if (period.m_data.m_nurses.Size() % yDelta)
		numberOfYPages++;
	return numberOfXPages * numberOfYPages;

}

void ScheduleViewEx::OnPrevious()
{
	m_offset--;
	Truncate(m_offset,0,TotalPages()-1);
	Invalidate();
}

void ScheduleViewEx::OnNext()
{
	m_offset++;
	Truncate(m_offset,0,TotalPages()-1);
	Invalidate();
}

void ScheduleViewEx::OnPrintAll()
{
	CPrintDialog dlg(FALSE);
	if (IDOK == dlg.DoModal())
	{
		HDC dc = dlg.GetPrinterDC();
		
		CDC DC;
		DC.Attach(dc);

		DEVMODE *myMode = dlg.GetDevMode();//fills myMode with printer defaults 
		myMode->dmOrientation = DMORIENT_LANDSCAPE;//change default to landscape
		myMode->dmPrintQuality = DMRES_DRAFT;
		myMode->dmColor = DMCOLOR_MONOCHROME;
		DC.ResetDC(myMode);

		DC.m_bPrinting = TRUE;
		OnPrepareDC(&DC);
		DC.StartDoc(_T("myDoc"));
		
		int tmpOffset = m_offset;
		for (int i = 0;
			i< TotalPages();
			i++)
		{
			m_offset = i;
			DC.StartPage();
			OnDraw(&DC);
			DC.EndPage();
		}
		DC.EndDoc();
		m_offset = tmpOffset;
	}
}