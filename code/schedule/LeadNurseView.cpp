#include "stdafx.h"
#include "leadnurseview.h"
#include "schedule.h"
#include "functions.h"

static const int width = 11000;
static const int height = 8500;

IMPLEMENT_DYNCREATE(LeadNurseView,CScrollView)

BEGIN_MESSAGE_MAP(LeadNurseView, CScrollView)
ON_WM_CREATE()
ON_COMMAND(ID_PREVIOUS, OnPreviousDay)
ON_COMMAND(ID_NEXT, OnNextDay)
ON_COMMAND(ID_FAST_FORWARD, OnNextWeek)
ON_COMMAND(ID_FAST_REVERSE, OnPreviousWeek)
ON_COMMAND(ID_PRINT_ONE, OnPrintOne)
ON_COMMAND(ID_PRINTALL, OnPrintall)
END_MESSAGE_MAP()

void LeadNurseView::OnPreviousDay()
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date - CTimeSpan(1,0,0,0)) >= period.m_dateRange.GetStartDate())
	m_date -= CTimeSpan(1,0,0,0);
	Invalidate();

}
void LeadNurseView::OnNextDay()
{
		Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date + CTimeSpan(1,0,0,0)) < period.m_dateRange.GetEndDate())
		m_date += CTimeSpan(1,0,0,0);
	Invalidate();

}
void LeadNurseView::OnNextWeek()
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date + CTimeSpan(7,0,0,0)) < period.m_dateRange.GetEndDate())
		m_date += CTimeSpan(7,0,0,0);
	else
		m_date = period.m_dateRange.GetEndDate() - CTimeSpan(1,0,0,0);
	
	Invalidate();
}
void LeadNurseView::OnPreviousWeek()
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date - CTimeSpan(7,0,0,0)) > period.m_dateRange.GetStartDate())
		m_date -= CTimeSpan(7,0,0,0);
	else
		m_date = period.m_dateRange.GetStartDate();
	Invalidate();
}
void LeadNurseView::OnPrintOne()
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
		DC.StartPage();
		
		OnDraw(&DC);
		DC.EndPage();
		DC.EndDoc();
	}
}

void LeadNurseView::OnPrintWeek()
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
		CTime lastDay = m_date += CTimeSpan(7,0,0,0);
		
		for (
			;m_date<lastDay;
			m_date+= CTimeSpan(1,0,0,0))
		{
			DC.StartPage();
			OnDraw(&DC);
			DC.EndPage();
		}
		DC.EndDoc();
		m_date -= CTimeSpan(7,0,0,0);
	}
}

void LeadNurseView::OnPrintall()
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
		Period& period = 
				static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
		CTimeSpan span = period.m_dateRange.GetEndDate() - period.m_dateRange.GetStartDate();
		int days = (int)span.GetDays();
		CTime lastDay = period.m_dateRange.GetEndDate();
		m_date = period.m_dateRange.GetStartDate();
		for (m_date=period.m_dateRange.GetStartDate()
			;m_date<lastDay;
			m_date+= CTimeSpan(1,0,0,0))
		{
			DC.StartPage();
			OnDraw(&DC);
			DC.EndPage();
		}
		DC.EndDoc();
	}
}


LeadNurseView::LeadNurseView():
m_date(GetPeriod().m_dateRange.GetStartDate())
{
}
int LeadNurseView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnInitialUpdate();

	return 0;
}

void LeadNurseView::OnInitialUpdate()
{
	SetScrollSizes(MM_HIENGLISH,CSize(width,height));
	GetParent()->SetWindowText(_T("Lead Nurse View"));
	CScrollView::OnInitialUpdate();
}
const int leftMargin = 10;
const int rightMargin = width/2;
const int halfMargin = (rightMargin-leftMargin)/2;
const int topMargin = -500;
const int numberOfLines = 50;
const int lineHeight = (height+topMargin)/numberOfLines;
const int bottomMargin = height * -1;
const int threeQuarters = rightMargin + halfMargin;
const int fullMargin = width -10;
void CreateBoldFont(CFont& font)
{
	CFont arial;
	VERIFY(arial.CreatePointFont(800,_T("Arial")));
	LOGFONT lf;
	VERIFY(arial.GetLogFont(&lf));
	lf.lfWeight = FW_BOLD;
	VERIFY(font.CreateFontIndirect(&lf));
}


void PlaceText(CDC * pDC,int x,int y,const CString& text,DWORD alignment,bool bold = false)
{
	CFont arialBold;
	VERIFY(arialBold.CreatePointFont(800,_T("Arial Bold")));
	int left = leftMargin;
	int right = halfMargin;
	if (x == 1)
	{
		left = halfMargin;
		right = rightMargin;
	}
	if (x == 2)
	{
		left = rightMargin;
		right = threeQuarters;
	}
	if (x==3)
	{
		left = threeQuarters;
		right = fullMargin;
	}

	left+= 30;
	right-=30;
	
	CFont * oldFont =NULL;
	if (bold)
		oldFont = pDC->SelectObject(&arialBold);
		pDC->DrawText(text,
		CRect(left,topMargin-lineHeight*(y+1)-15,right,
		topMargin-lineHeight*(y+2)-15),
			DT_NOCLIP | alignment);
	if (bold)
		pDC->SelectObject(oldFont);

}
void LeadNurseView::OnDraw(CDC *pDC)
{
	vector<ShiftSetup> setups = GetShifts(GetPeriod(),this->m_date);

	CPen pen(PS_SOLID,5,RGB(0,0,0));
	CPen * oldPen = pDC->SelectObject(&pen);

	CFont arial;
	VERIFY(arial.CreatePointFont(800,_T("Arial")));
	CFont arialBold;
	VERIFY(arialBold.CreatePointFont(800,_T("Arial Bold")));
	CFont * oldFont = pDC->SelectObject(&arial);
	pDC->MoveTo(halfMargin,topMargin-lineHeight);
	pDC->LineTo(halfMargin,bottomMargin);
	pDC->MoveTo(rightMargin,topMargin-lineHeight);
	pDC->LineTo(rightMargin,bottomMargin);
	pDC->MoveTo(threeQuarters,topMargin-lineHeight);
	pDC->LineTo(threeQuarters,bottomMargin);
	pDC->MoveTo(fullMargin,topMargin-lineHeight);
	pDC->LineTo(fullMargin,bottomMargin);
	for (int i=0;i<numberOfLines;i++)
	{
		const int currentY = topMargin - lineHeight*i;
		pDC->MoveTo(leftMargin,currentY);
		pDC->LineTo(fullMargin,currentY);
	}
	CString dateText = GetDayName() + _T(" ") + GetDate();
	VERIFY(&arial == pDC->SelectObject(&arialBold));
	pDC->DrawText(dateText,CRect(leftMargin,topMargin-10,
							fullMargin,topMargin -lineHeight-10),
		DT_NOCLIP | DT_CENTER);
	VERIFY(&arialBold == pDC->SelectObject(&arial));
	PlaceText(pDC,0,0,_T("0700-1100"),DT_CENTER,true);
	PlaceText(pDC,1,0,_T("1100-1530"),DT_CENTER,true);
	vector<CString> names = GetWorkersAtStartTime(setups,this->m_date,7);
	for (int i=0;i<11;i++)
	{
		CString num;
		num.Format(_T("%d) "),i+1);
		if (i < names.size())
			num += _T(" ") + names[i];
		PlaceText(pDC,0,i+1,num,DT_LEFT);
	}
	names = GetWorkersAtStartTime(setups,this->m_date,11);
	for (int i=0;i<11;i++)
	{
		CString num;
		num.Format(_T("%d)"),i+1);
		if (i < names.size())
			num += _T(" ") + names[i];
		
		PlaceText(pDC,1,i+1,num,DT_LEFT);
	}
	const int firstTechnicianPart = 13;
	PlaceText(pDC,0,13,_T("Technicians"),DT_CENTER,true);
	for (int i=0;i<2;i++)
	{
		CString num;
		num.Format(_T("%d)"),i+1);
		PlaceText(pDC,0,firstTechnicianPart+ 1 + i,num,DT_LEFT);
		PlaceText(pDC,1,firstTechnicianPart+ 1 + i,num,DT_LEFT);
	}

	const int secondPart = 17;
	PlaceText(pDC,0,secondPart,_T("1500-1900"),DT_CENTER,true);
	names = GetWorkersAtStartTime(setups,this->m_date,15);
	for (int i=0;i<13;i++)
	{
		CString num;
		num.Format(_T("%d)"),i+1);
		if (i < names.size())
			num += _T(" ") + names[i];
		PlaceText(pDC,0,secondPart + 1 + i,num,DT_LEFT);
	}
	PlaceText(pDC,1,secondPart,_T("1900-2300"),DT_CENTER,true);
	names = GetWorkersAtStartTime(setups,this->m_date,19);
	for (int i=0;i<13;i++)
	{
		CString num;
		num.Format(_T("%d)"),i+1);
		if (i < names.size())
			num += _T(" ") + names[i];
		PlaceText(pDC,1,secondPart + 1 + i,num,DT_LEFT);
	}
	const int secondTechnicianPart = secondPart + 14;
	PlaceText(pDC,0,secondTechnicianPart,_T("Technicians"),DT_CENTER,true);
	for (int i=0;i<3;i++)
	{
		CString num;
		num.Format(_T("%d)"),i+1);
		PlaceText(pDC,0,i+secondTechnicianPart+ 1,num,DT_LEFT);
		PlaceText(pDC,1,i+secondTechnicianPart+ 1,num,DT_LEFT);
	}
	const int thirdPart = secondTechnicianPart + 5;
	PlaceText(pDC,0,thirdPart,_T("2300-0700"),DT_CENTER,true);
	names = GetWorkersAtStartTime(setups,m_date,23,31);
	for (int i=0;i<5;i++)
	{
		CString num;
		num.Format(_T("%d)"),i+1);
		if (i < names.size())
			num += names[i];
		PlaceText(pDC,0,thirdPart + i + 1,num,DT_LEFT);
		num.Format(_T("%d)"),i+1+5);
		if (i+5 < names.size())
			num += names[i+5];
		PlaceText(pDC,1,thirdPart + i + 1,num,DT_LEFT);
	}
	const int thirdTechnicianPart = thirdPart + 9;
	PlaceText(pDC,0,thirdTechnicianPart,_T("Technicians"),DT_CENTER,true);
	for (int i=0;i<1;i++)
	{
		CString num;
		num.Format(_T("%d)"),i+1);
		PlaceText(pDC,0,i+thirdTechnicianPart+1,num,DT_LEFT);
		num.Format(_T("%d)"),i+1+1);
		PlaceText(pDC,1,i+thirdTechnicianPart + 1,num,DT_LEFT);
	}

	PlaceText(pDC,2,0,_T("***Lead Nurse Messages***"),DT_CENTER,true);
	PlaceText(pDC,3,0,_T("Staffing Needs"),DT_CENTER,true);
	PlaceText(pDC,3,1,_T("\"Over Staff\""),DT_CENTER,true);
	PlaceText(pDC,3,6,_T("\"Needs\""),DT_CENTER,true);
	PlaceText(pDC,3,11,_T("Voluntary Availability"),DT_CENTER,true);
	PlaceText(pDC,3,17,_T("Required Availability"),DT_CENTER,true);
	PlaceText(pDC,3,38,_T("Sick Call"),DT_CENTER,true);
	PlaceText(pDC,2,23,_T("Orientation"),DT_CENTER,true);
	for (int i=0;i<4;i++)
	{
		PlaceText(pDC,2,24+i*5,_T("Time"),DT_LEFT);
		PlaceText(pDC,2,25+i*5,_T("Orientee"),DT_LEFT);
		PlaceText(pDC,2,26+i*5,_T("Preceptor"),DT_LEFT);
		PlaceText(pDC,2,27+i*5,_T("Pod"),DT_LEFT);
	}
	
	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldPen);
	
}