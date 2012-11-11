// DailyAssignmentsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DailyAssignmentsDlg.h"
#include "MyException.h"
#include "functions.h"
#include ".\dailyassignmentsdlg.h"
// DailyAssignmentsDlg dialog

DailyAssignmentsDlg::DailyAssignmentsDlg(Period& period)
	: CDialog(DailyAssignmentsDlg::IDD, NULL),
	m_oldPeriod(period),
	m_period(period),
	m_date(0)
	, m_startTimeCtrl(0)
	, m_numberOfHoursCtrl(0)
	, m_isOverrideOn(false)
{
}

DailyAssignmentsDlg::DailyAssignmentsDlg(Period& period,int date)
	: CDialog(DailyAssignmentsDlg::IDD, NULL),
	m_oldPeriod(period),
	m_period(period),
	m_date(date),
	m_startTimeCtrl(0),
	m_numberOfHoursCtrl(0),
	m_isOverrideOn(false)
{
}


void DailyAssignmentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_LIST, m_nameListCtrl);
	DDX_Control(pDX, IDC_SCHEDULE_LIST, m_scheduleListCtrl);
	DDX_Control(pDX, IDC_DATE, m_dateCtrl);
	DDX_Text(pDX, IDC_START_TIME, m_startTimeCtrl);
	DDX_Text(pDX, IDC_NUMBER_OF_HOURS, m_numberOfHoursCtrl);
	DDX_Control(pDX, IDC_OVERRIDE_LIST, m_overrideList);
}


BEGIN_MESSAGE_MAP(DailyAssignmentsDlg, CDialog)
	ON_NOTIFY(MCN_SELCHANGE, IDC_DATE, OnMcnSelchangeDate)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_CHANGE, OnBnClickedChange)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_LBN_SELCHANGE(IDC_NAME_LIST, OnLbnSelchangeNameList)
	ON_BN_CLICKED(IDC_ENABLE_FIXED, OnBnClickedEnableFixed)
END_MESSAGE_MAP()

static 
void SetCalendar(const DateRange& dateRange,
				   CMonthCalCtrl& calCtrl,
				   int days)
{
	CTime startTime = dateRange.GetStartDate();
	calCtrl.SetCurSel(startTime + CTimeSpan(days,0,0,0));
}

static
int GetCalendar(const DateRange& dateRange,
				CMonthCalCtrl& calCtrl)
{
	CTime selected;
	calCtrl.GetCurSel(selected);
	CTimeSpan span = selected - dateRange.GetStartDate();
	CTimeSpan fullSpan = dateRange.GetEndDate() - dateRange.GetStartDate();
	if (span.GetDays() < 0)
		Throw(_T("The selected date is before the time period."));
	if (span.GetDays() > fullSpan.GetDays())
		Throw(_T("The selected date is after the time period."));
	return static_cast<int>(span.GetDays());
}

void DailyAssignmentsDlg::UpdateControls()
{
	// add each name to the name list
	const Nurses& nurses = m_period.m_data.m_nurses;
	m_nameListCtrl.ResetContent();
	for (int i=0;i<nurses.Size();i++)
	{
		m_nameListCtrl.AddString(nurses[i].GetName().FullName());
	}

	SetCalendar(m_period.m_dateRange,m_dateCtrl,m_date);

	DailyOverrides&
		assignments = m_period.m_data.m_assignments[m_date];
	m_scheduleListCtrl.ResetContent();
	for (int i=0;i<assignments.Size();i++)
	{
		m_scheduleListCtrl.AddString(assignments[i].ToString());
	}
}

void DailyAssignmentsDlg::UpdateFixed()
{
	m_overrideList.ResetContent();
	vector<CString> status = 
		GetAllFixedScheduleStatus(m_period,m_date);
	for (int i=0;i<status.size();i++)
	{
		m_overrideList.AddString(status[i]);
	}
}

BOOL DailyAssignmentsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateControls();
	UpdateFixed();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DailyAssignmentsDlg::OnOK()
{
	m_oldPeriod = m_period;
	CDialog::OnOK();
}

void DailyAssignmentsDlg::OnMcnSelchangeDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	try
	{
		m_date = GetCalendar(m_period.m_dateRange,m_dateCtrl); 
	}
	catch(...)
	{
	}
	//MY_CATCH()
	SetCalendar(m_period.m_dateRange,m_dateCtrl,m_date);
	UpdateControls();
	UpdateFixed();



	*pResult = 0;
}

void DailyAssignmentsDlg::OnBnClickedAdd()
{
	DailyOverrides&
		assignments = m_period.m_data.m_assignments[m_date];
	
	UpdateData(TRUE);

	try
	{
		int selected = m_nameListCtrl.GetCurSel();
		if (selected == -1)
			return;
		const Nurses& nurses = m_period.m_data.m_nurses;

		if (m_startTimeCtrl < 0 || 
			m_startTimeCtrl > 24)
			Throw(_T("Start time is out of range"));

		assignments.Add(Assignment
			(nurses[selected].GetName(),m_startTimeCtrl,
						m_numberOfHoursCtrl));
	}
	MY_CATCH()
	UpdateControls();

}

void DailyAssignmentsDlg::OnBnClickedChange()
{
	// TODO: Add your control notification handler code here
}

void DailyAssignmentsDlg::OnBnClickedRemove()
{
	int selected = m_scheduleListCtrl.GetCurSel();
	if (selected == -1)
		return;

	DailyOverrides&
		assignments = m_period.m_data.m_assignments[m_date];
	assignments.Remove(selected);
	UpdateControls();
}

void DailyAssignmentsDlg::OnLbnSelchangeNameList()
{
	int selected = m_nameListCtrl.GetCurSel();
	if (selected == -1)
		return;
	Nurse& nurse = m_period.m_data.m_nurses[selected];
	m_startTimeCtrl = nurse.GetStartTime();
	m_numberOfHoursCtrl = nurse.GetDailyHours();
	UpdateData(FALSE);
}

void DailyAssignmentsDlg::OnBnClickedEnableFixed()
{
	int selected = m_overrideList.GetCurSel();
	if (selected == -1)
		return;

	bool isEnabled = GetFixedScheduleStatus(m_period,
		m_date,selected);
	SetFixedScheduleStatus(m_period,m_date,selected,!isEnabled);
	UpdateFixed();
}

void DailyAssignmentsDlg::OnBnClickedEditVacations()
{
	// TODO: Add your control notification handler code here
}
