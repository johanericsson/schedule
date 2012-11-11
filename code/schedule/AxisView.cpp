#include "stdafx.h"
#include "AxisView.h"
#include "Period.h"
#include "Schedule.h"
#include "DisplayHelp.h"

BEGIN_MESSAGE_MAP(AxisView,CWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

AxisView::AxisView():
m_bFirstSize(false),
m_scheduleView(NULL),
m_height(0),
m_width(0)
{
}

void AxisView::OnNcDestroy()
{
	delete this;
}


void AxisView::OnSize(UINT nType,int cx,int cy)
{
	CWnd::OnSize(nType,cx,cy);
	if (m_bFirstSize++)
	{
		m_scheduleView->MoveWindow(m_width,m_height,
			cx-m_width,cy-m_height);
	}
}

int AxisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) ==-1)
		return -1;
	ASSERT(!m_scheduleView);
	m_scheduleView = new ScheduleView;
	VERIFY(m_scheduleView->Create(NULL, NULL, WS_HSCROLL | WS_VSCROLL | 
		AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, 1234, NULL));
	m_scheduleView->ShowWindow(SW_SHOW);

	return 0;
}

static int LeftSide(const Nurses& nurses)
{
	int maxLength = 0;
	for (int i=0;i<nurses.Size();i++)
	{
		maxLength = max(nurses[i].GetName().FullName().GetLength(),maxLength);
	}
	return maxLength;
}

//|01|02|03|04|05
//|26|27|28|29|30
//|Mo|Tu|We|Th|Fr
static char* dates[] = 
{
	"Su",
	"Mo",
	"Tu",
	"We",
	"Th",
	"Fr",
	"Sa"
};

static CString ToString(int i)
{
	CString str;
	str.Format(_T("0%d"),i);
	if (str.GetLength() == 3)
	{
		str.Delete(0);
	}
	return str;
}
static CString FormatForThirds(CString in)
{
	CString front;
//	front.SetLength(LeftSide());
	


}


static CString BeginHeader(Data& data)
{
	CString str;
	for(int i=0;i<LeftSide(data.m_nurses);i++)
	{
		str += ' ';
	}
	str += '|';
	return str;
}

static CString ViewOutput()
{

	Periods& periods = 
		static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	if (periods.GetCurrent()==-1)
		return _T("No period selected");
	
	Period& period = periods.GetCurrentPeriod();
	CTime startTime = period.m_dateRange.GetStartDate();
	CTime endTime = period.m_dateRange.GetEndDate();

	Data& data = period.m_data;
	CString Months= BeginHeader(data);
	CString Dates= BeginHeader(data);
	CString Days= BeginHeader(data);

	vector<CString> nurseSchedules(data.m_nurses.Size());
	for (int i=0;i<nurseSchedules.size();i++)
	{
		nurseSchedules[i] = BeginHeader(data);
		for(int j=0;j<data.m_nurses[i].GetName().FullName().GetLength();j++)
		{
			nurseSchedules[i].GetBuffer(0)[j] = data.m_nurses[i].GetName().FullName()[j];
			nurseSchedules[i].ReleaseBuffer();
		}
	}
	
	// for each day
	for(;startTime < endTime;startTime += CTimeSpan(1,0,0,0))
	{
		Months += ToString(startTime.GetMonth());
		Months += '|';
		Dates += ToString(startTime.GetDay());
		Dates += '|';
		Days += dates[startTime.GetDayOfWeek()-1];
		Days += '|';

		// for each nurse
		for (int i =0;i<data.m_nurses.Size();i++)
		{
			Nurse& nurse = data.m_nurses[i];
			nurseSchedules[i] += nurse.IsWorkingOn(startTime,data.m_legend);
			nurseSchedules[i] += '|';
		}

	}
	CString divider;
	for (int i=0;i<Months.GetLength();i++)
	{
		divider += '-';
	}
	CString compositeNurses;
	for (int i=0;i<nurseSchedules.size();i++)
	{
		compositeNurses += nurseSchedules[i];
		compositeNurses += '\n';
	}
	return Months + '\n' + 
		   Dates + '\n' + 
		   Days + '\n' + 
		   divider + '\n' +
		   compositeNurses;
}

static CString GetDates()
{
	Periods& periods = 
		static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	if (periods.GetCurrent()==-1)
		return _T("");
	
	Period& period = periods.GetCurrentPeriod();
	CTime startTime = period.m_dateRange.GetStartDate();
	CTime endTime = period.m_dateRange.GetEndDate();

	Data& data = period.m_data;
	CString Months= BeginHeader(data);
	CString Dates= BeginHeader(data);
	CString Days= BeginHeader(data);
	// for each day
	for(;startTime < endTime;startTime += CTimeSpan(1,0,0,0))
	{
		Months += ToString(startTime.GetMonth());
		Months += '|';
		Dates += ToString(startTime.GetDay());
		Dates += '|';
		Days += dates[startTime.GetDayOfWeek()-1];
		Days += '|';
	}
	return Months + _T("\n") +
		   Dates + _T("\n") +
		   Days + _T("\n");
}


static CString GetNames()
{
	Periods& periods = 
		static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	if (periods.GetCurrent()==-1)
		return _T("No period selected");
	
	Period& period = periods.GetCurrentPeriod();
	CTime startTime = period.m_dateRange.GetStartDate();
	CTime endTime = period.m_dateRange.GetEndDate();

	Data& data = period.m_data;
	
	CString nurses;
	for (int i=0;i<data.m_nurses.Size();i++)
	{
		nurses += data.m_nurses[i].GetName().FullName();
		nurses += _T('\n');
	}
	return nurses;

}

void AxisView::OnPaint()
{
	CClientDC dc(this);
	EraseBackground(&dc,this);
	CString strDates = GetDates();
	CString strNames = GetNames();
	if (strDates.IsEmpty() || strNames.IsEmpty())
		return;
	CFont courier;
	VERIFY(courier.CreatePointFont(120,_T("Courier New")));
	CFont * oldFont = dc.SelectObject(&courier);

	CRect rcOut(0,0,0,0);
	VERIFY(dc.DrawText(strDates,&rcOut,DT_NOCLIP | DT_CALCRECT));
	m_height = rcOut.Height();
	VERIFY(dc.DrawText(strNames,&rcOut,DT_NOCLIP | DT_CALCRECT));
	m_width = rcOut.Width();

	CRect rcWind;
	GetClientRect(&rcWind);
	dc.FillRect(CRect(0,0,rcWind.Width(),m_height)
		,&CBrush(RGB(255,255,255)));
	dc.FillRect(CRect(0,0,m_width,rcWind.Height())
		,&CBrush(RGB(255,255,255)));



	VERIFY(dc.DrawText(strDates,CRect(0,0,0,0),DT_NOCLIP));
	VERIFY(dc.DrawText(strNames,CRect(0,m_height,0,m_height),
		DT_NOCLIP));
	VERIFY(&courier == dc.SelectObject(oldFont));
}