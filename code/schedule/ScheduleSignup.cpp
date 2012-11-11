// ScheduleSignup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Data.h"
#include "ScheduleSignup.h"
#include ".\schedulesignup.h"


// ScheduleSignup dialog
ScheduleSignup::ScheduleSignup(Data& d)
:m_data(d),
CDialog(ScheduleSignup::IDD, NULL)
{
}


void ScheduleSignup::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_CALENDAR, m_Calendar);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ScheduleSignup, CDialog)
	ON_NOTIFY(MCN_SELCHANGE, IDC_CALENDAR, OnMcnSelchangeCalendar)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// ScheduleSignup message handlers

BOOL ScheduleSignup::OnInitDialog()
{
	CDialog::OnInitDialog();



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void ScheduleSignup::OnMcnSelchangeCalendar(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateAll();
	*pResult = 0;
}

void FillCombo(CComboBox& c,const Nurses& nurses)
{
	c.ResetContent();
	for (int i=0;i<nurses.Size();i++)
	{
		c.AddString(nurses[i].GetName().FullName());
	}
}

void ScheduleSignup::FreeAll()
{
	for (int i=0;i<m_shiftTitles.size();i++)
		delete m_shiftTitles[i];

	m_shiftTitles.clear();
	for (int i=0;i<m_shifts.size();i++)	
		for (int j=0;j<m_shifts[i].size();j++)
			delete m_shifts[i][j];
	m_shifts.clear();
}

void ScheduleSignup::UpdateAll()
{
	FreeAll();
	CWnd * pWnd = GetDlgItem(IDC_SIGNUP);
	CRect fullRect;
	pWnd->GetWindowRect(&fullRect);
	ScreenToClient(&fullRect);
	CTime time;
	VERIFY(m_Calendar.GetCurSel(time));
	WeekDayTemplate weekDay = 
		m_data.m_ShiftsTemplate[time.GetDayOfWeek()-1];
	// for each shift
	int currentHeight = fullRect.top;
	for (int i=0;i<weekDay.Size();i++)
	{
		CRect thisStatic;
		thisStatic.right = fullRect.right;
		thisStatic.left = fullRect.left;
		thisStatic.top = currentHeight;
		thisStatic.bottom = currentHeight + 20;
		m_shiftTitles.push_back(new CStatic);
		CString title;
		title.Format("Hour: %d",weekDay[i].startTime);
		VERIFY(m_shiftTitles[i]->Create(title,WS_VISIBLE,thisStatic,this));
		
		m_shifts.push_back(vector<CComboBox*>());
		ShiftTemplate templ = weekDay[i];
		currentHeight+=22;
		for(int j=0;j<templ.nurses;j++)
		{
			CComboBox * p = new CComboBox();
			m_shifts[i].push_back(p);
			CRect thisCombo(0,0,0,0);
			thisCombo.right = fullRect.right;
			thisCombo.left = fullRect.left;
			thisCombo.top = currentHeight;
			thisCombo.bottom = currentHeight+20;
			VERIFY(p->Create(WS_VISIBLE | CBS_DROPDOWN | CBS_SORT | 
                    WS_VSCROLL | WS_TABSTOP,thisCombo,this,1234));
			FillCombo(*p,m_data.m_nurses);
			currentHeight+=26;
		}
	}
	UpdateData(FALSE);

}

void ScheduleSignup::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	UpdateAll();
}
