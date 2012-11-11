// ChooseNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "ChooseNameDlg.h"
#include "functions.h"
#include ".\choosenamedlg.h"


// ChooseNameDlg dialog

ChooseNameDlg::ChooseNameDlg(const vector<Nurse>& nurses,
							 CTime date)
	: CDialog(ChooseNameDlg::IDD, NULL),
	m_nurses(nurses),
	m_date(date)
{
}

void ChooseNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_LIST, m_nameListCtrl);
}


BEGIN_MESSAGE_MAP(ChooseNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// ChooseNameDlg message handlers

BOOL ChooseNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nameListCtrl.InsertColumn(0,_T("Name"));
	VERIFY(m_nameListCtrl.InsertColumn(1,_T("Start time")));
	VERIFY(m_nameListCtrl.InsertColumn(2,_T("Hours this week")));
	VERIFY(m_nameListCtrl.InsertColumn(3,_T("Hours needed")));
	
	
	
	Period& period = 
		static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	int dayNumber = (int)(m_date - period.m_dateRange.GetStartDate()).GetDays();
	int weekNumber = dayNumber/7;



	vector<int> hoursUsed = 
		GetNumberOfHoursPerWeek(period,m_nurses,weekNumber);

	for (int i=0;i<m_nurses.size();i++)
	{
		const Nurse& nurse = m_nurses[i];
		m_nameListCtrl.InsertItem(i,nurse.GetName().FullName());
		CString tmp;
		tmp.Format(_T("%d"),nurse.GetStartTime());
		m_nameListCtrl.SetItem(i,1,LVIF_TEXT,tmp,0,0,0,0);
		tmp.Format(_T("%d"),hoursUsed[i]);
		m_nameListCtrl.SetItem(i,2,LVIF_TEXT,tmp,0,0,0,0);
		tmp.Format(_T("%d"),nurse.GetWeeklyHours());
		m_nameListCtrl.SetItem(i,3,LVIF_TEXT,tmp,0,0,0,0);
	}

	m_nameListCtrl.SetColumnWidth(0,LVSCW_AUTOSIZE);
	m_nameListCtrl.SetColumnWidth(1,100);
	m_nameListCtrl.SetColumnWidth(2,100);
	m_nameListCtrl.SetColumnWidth(3,100);
	
	return TRUE;
}

Optional<Nurse> ChooseNurse(const vector<Nurse>& nurses,CTime date)
{
	ChooseNameDlg dlg(nurses,date);
	int result = dlg.DoModal();
	if (result != IDOK)
		return Empty();
	int selected = dlg.m_selectedIndex;
	if (selected == -1)
		return Empty();
	return nurses[selected];
}

void ChooseNameDlg::OnBnClickedOk()
{
	m_selectedIndex = m_nameListCtrl.GetSelectionMark();
	OnOK();
}
