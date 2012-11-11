// ScheduleOverrideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "ScheduleOverrideDlg.h"
#include ".\scheduleoverridedlg.h"
#include "myexception.h"


// ScheduleOverrideDlg dialog


ScheduleOverrideDlg::ScheduleOverrideDlg(int dayNumber)
	: CDialog(ScheduleOverrideDlg::IDD, NULL),
	m_dayNumber(dayNumber)
	, m_startTimeCtrl(1)
	, m_numberOfSlots(0),
	m_schedules(GetPeriod().m_data.m_assignments[dayNumber].m_temporarySchedule)
	, m_dayName(_T(""))
{
	CTime day = GetPeriod().m_dateRange.GetStartDate() + CTimeSpan(dayNumber,0,0,0);
	m_dayName = day.Format(_T("%A, %B %d, %Y"));
}

void ScheduleOverrideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OVERRIDES, m_overridesListCtrl);

	if (!pDX->m_bSaveAndValidate)
	{
		m_overridesListCtrl.ResetContent();
		for (int i=0;i<m_schedules.size();i++)
		{
			CString str;
			str.Format(_T("Start time:%d Slots:%d"),m_schedules[i].startTime,
				m_schedules[i].numberOfSlots);
			m_overridesListCtrl.AddString(str);
		}
	}
	DDX_Text(pDX, IDC_START_TIME, m_startTimeCtrl);
	DDV_MinMaxInt(pDX, m_startTimeCtrl, 1, 23);
	DDX_Text(pDX, IDC_NUMBER_OF_SLOTS, m_numberOfSlots);
	DDV_MinMaxInt(pDX, m_numberOfSlots, 0, 10);
	DDX_Text(pDX, IDC_DAY_NAME, m_dayName);
}


BEGIN_MESSAGE_MAP(ScheduleOverrideDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// ScheduleOverrideDlg message handlers

void ScheduleOverrideDlg::OnBnClickedAdd()
{
	UpdateData(TRUE);
	TemporarySchedule newSchedule(m_startTimeCtrl,m_numberOfSlots);
	vector<TemporarySchedule>::const_iterator i = 
		std::find(m_schedules.begin(),m_schedules.end(),newSchedule);
	// If this already exists, then we don't add it.
	if (i!=m_schedules.end())
		return;
	m_schedules.push_back(newSchedule);
	std::sort(m_schedules.begin(),m_schedules.end());
	UpdateData(FALSE);
}

void ScheduleOverrideDlg::OnBnClickedRemove()
{
	UpdateData(TRUE);
	int selection = m_overridesListCtrl.GetCurSel();
	if (selection == LB_ERR)
		return;

	m_schedules.erase(m_schedules.begin() + selection);
	UpdateData(FALSE);
}

void ScheduleOverrideDlg::OnBnClickedOk()
{
	OnOK();
	GetPeriod().m_data.m_assignments[m_dayNumber].
		m_temporarySchedule = m_schedules;
	
}
