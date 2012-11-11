// ScheduleView.cpp : implementation of the CScheduleView class
//

#include "stdafx.h"
#include "ScheduleView.h"
#include "Schedule.h"
#include "functions.h"
#include ".\scheduleview.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(ScheduleView, CScrollView)
// CScheduleView

BEGIN_MESSAGE_MAP(ScheduleView, CScrollView)
	ON_WM_NCDESTROY()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_PRINTPREVIEW,OnFilePrintPreview)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static int LeftSide(const Nurses& nurses)
{
	int maxLength = 0;
	for (int i=0;i<nurses.Size();i++)
	{
		maxLength = max(nurses[i].GetName().LastFirst().GetLength(),maxLength);
	}
	return maxLength+ 7;
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

CString ToString(int i)
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


static CString BeginHeader(Data& data,int leftSide)
{
	CString str;
	str.Preallocate(leftSide+10);
	for(int i=0;i<leftSide;i++)
	{
		str += ' ';
	}
	str += '|';
	return str;
}

int WhichShift(const Nurse& n)
{
	int startTime = n.GetStartTime();
	if (startTime <= 0)
		return 3;
	if (startTime < 11)
		return 0;
	if (startTime < 19)
		return 1;
	return 2;

}

const CString shiftChangeStr("Shift--- --- Next");
bool SortByShift(const Nurse& n1,const Nurse& n2)
{
	if (WhichShift(n1) < WhichShift(n2))
		return true;
	if (WhichShift(n1) > WhichShift(n2))
		return false;

	if (n1.GetName().FullName() == shiftChangeStr)
		return true;

	return (n1.GetName() < n2.GetName());
}

template<class T>
static void Truncate(T& value,T min,T max)
{
	if (value > max)
		value = max;
	if (value < min)
		value = min;
}

CString ViewOutput(CTime startTime,CTime endTime,
				   int firstName,int lastName)
{	

	Periods& periods = static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	if (periods.GetCurrent()==-1)
		return _T("No period selected");
	
	Period& period = periods.GetCurrentPeriod();
	Data& data = period.m_data;

	vector<Nurse> tmpNurses = data.m_nurses.m_nurses;
	tmpNurses.push_back(Nurse(Name(shiftChangeStr),8,32,11));

	tmpNurses.push_back(Nurse(Name("  "),8,32,11));
	tmpNurses.push_back(Nurse(Name("  "),8,32,11));

	tmpNurses.push_back(Nurse(Name(shiftChangeStr),8,32,19));
	tmpNurses.push_back(Nurse(Name("  "),8,32,19));
	tmpNurses.push_back(Nurse(Name("  "),8,32,19));

	tmpNurses.push_back(Nurse(Name(shiftChangeStr),8,32,0));
	tmpNurses.push_back(Nurse(Name("  "),8,32,0));
	tmpNurses.push_back(Nurse(Name("  "),8,32,0));

	std::sort(tmpNurses.begin(),tmpNurses.end(), &SortByShift);

	//CTime startTime = period.m_dateRange.GetStartDate();
	//CTime endTime = period.m_dateRange.GetEndDate();

	int leftSide = LeftSide(data.m_nurses);
	CString Months= BeginHeader(data,leftSide);
	CString Dates= BeginHeader(data,leftSide);
	CString Days= BeginHeader(data,leftSide);

	Truncate(firstName,0,(int)tmpNurses.size());
	Truncate(lastName,0,(int)tmpNurses.size());
	ASSERT(firstName <= tmpNurses.size());
	ASSERT(lastName <= tmpNurses.size());
	ASSERT(firstName <= lastName);
	vector<CString> nurseSchedules(lastName - firstName);
	__int64 numberOfDays= (endTime-startTime).GetDays();

	for (int i=0;i<nurseSchedules.size();i++)
	{
		nurseSchedules[i] = BeginHeader(data,leftSide);
		nurseSchedules[i].Preallocate((int)numberOfDays*3);
		CString fullName = tmpNurses[i+firstName].GetName().LastFirst();
		int length = fullName.GetLength();
		TCHAR * pStr = nurseSchedules[i].GetBuffer(0);
		memcpy(pStr,fullName.GetBuffer(0),length * sizeof(TCHAR));
		
		
		// Put the classification at the end of the header
		CString classification = tmpNurses[i+firstName].GetClassification().GetBuffer(0);
		for (int j=0;j<4;j++)
		{
			int length = nurseSchedules[i].GetLength();
			if (j >= classification.GetLength())
				pStr[length - 5 + j] = ' ';
			else
				pStr[length - 5 + j] = classification[j];
		}
		nurseSchedules[i].ReleaseBuffer();
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

		vector<Slot> cachedResults;
		// for each nurse
		for (int i =0;i<nurseSchedules.size();i++)
		{
			Nurse& nurse = tmpNurses[i+firstName];
			nurseSchedules[i] += 
				GetWorkingNotation(period,
				startTime,nurse,
				cachedResults);
				//nurse.IsWorkingOn(startTime,data.m_legend);
			nurseSchedules[i] += '|';
		}

	}
	CString divider;
	for (int i=0;i<Months.GetLength();i++)
	{
		divider += '-';
	}

	CString compositeNurses;
	
	CString dashes;
	if (nurseSchedules.size())
	{
		int length = nurseSchedules[0].GetLength();
		dashes.Preallocate(length);
		for (int i=0;i<length;i++)
		{
			dashes += '-';
		}
		dashes+= '\n';
	}
	for (int i=0;i<nurseSchedules.size();i++)
	{
		compositeNurses += nurseSchedules[i];
		compositeNurses += '\n';
	//	compositeNurses += dashes;
	}
	return Months + '\n' + 
		   Dates + '\n' + 
		   Days + '\n' + 
		   divider + '\n' +
		   compositeNurses;

}

CString ViewAllOutput()
{

	Periods& periods = static_cast<CScheduleApp*>(AfxGetApp())->m_periods;
	Period& period = periods.GetCurrentPeriod();

	int totalNurses = period.m_data.m_nurses.Size();
	CTime startTime = period.m_dateRange.GetStartDate();
	CTime endTime = period.m_dateRange.GetEndDate();
	return ViewOutput(startTime,endTime,0,totalNurses);
}

void ScheduleView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT,CSize(100,100));
}

void ScheduleView::OnDraw(CDC * pDC)
{
	CDC& dc(*pDC);
	CRect rcView;
	GetWindowRect(&rcView);
	ScreenToClient(&rcView);
	CBrush brush(RGB(255,255,255));
	dc.FillRect(&rcView,&brush);
	CFont courier;
	VERIFY(courier.CreatePointFont(120,_T("Courier New")));
	CFont * oldFont = dc.SelectObject(&courier);
	//if (!m_strOutput)
		m_strOutput = ViewAllOutput();
	CString str = *m_strOutput;
	dc.DrawText(str,CRect(0,0,0,0),DT_NOCLIP);

	CRect rcOut(0,0,0,0);
	VERIFY(dc.DrawText(str,&rcOut,DT_NOCLIP | DT_CALCRECT));
	CSize newSize(rcOut.right,rcOut.bottom);
	if (newSize != m_oldSize)
	{
		SetScrollSizes(MM_TEXT,newSize);
		m_oldSize = newSize;
	}
	VERIFY(&courier == dc.SelectObject(oldFont));

}

void ScheduleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnLButtonDown(nFlags, point);
}

void ScheduleView::UpdateTheSubject()
{
	m_strOutput = ViewAllOutput();
	Invalidate();
}
int ScheduleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnInitialUpdate();

	return 0;
}
