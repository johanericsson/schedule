// FixedScheduleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FixedScheduleDlg.h"
#include "MyException.h"
#include "Weeknames.h"
#include ".\fixedscheduledlg.h"

// FixedScheduleDlg dialog

IMPLEMENT_DYNAMIC(FixedScheduleDlg, CDialog)
FixedScheduleDlg::
FixedScheduleDlg(CString name,FixedSchedule fixedSchedule,
				 int startTime,int hours)
	: CDialog(FixedScheduleDlg::IDD)
	, m_weekNumberCtrl(0)
	, m_dayNumberCtrl(0)
	, m_startTime(startTime)
	, m_hours(hours),
	m_fixedSchedule(fixedSchedule)
	, m_dayNameCtrl(0)
	, m_isOnlyAvailableCtrl(FALSE),
	m_name(name)
{
}


void FixedScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WEEK_NUMBER, m_weekNumberCtrl);
	DDX_Text(pDX, IDC_DAY_NUMBER, m_dayNumberCtrl);
	DDX_Text(pDX, IDC_START_TIME, m_startTime);
	DDX_Text(pDX, IDC_HOURS, m_hours);
	DDX_Control(pDX, IDC_FIXED_LIST, m_fixedListCtrl);
	DDX_CBIndex(pDX, IDC_DAY_NAME, m_dayNameCtrl);
	DDX_Check(pDX, IDC_AVAILABLE, m_isOnlyAvailableCtrl);
}


BEGIN_MESSAGE_MAP(FixedScheduleDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDC_CHANGE, OnBnClickedChange)
	ON_LBN_SELCHANGE(IDC_FIXED_LIST, OnLbnSelchangeFixedList)
	ON_EN_KILLFOCUS(IDC_DAY_NUMBER, OnEnKillfocusDayNumber)
	ON_CBN_SELCHANGE(IDC_DAY_NAME, OnCbnSelchangeDayName)
END_MESSAGE_MAP()


void FixedScheduleDlg::UpdateDialog()
{
	m_fixedListCtrl.ResetContent();
	for (int i=0;i<m_fixedSchedule.Size();i++)
	{
		m_fixedListCtrl.AddString(m_fixedSchedule[i].ToString());
	}
	UpdateData(FALSE);
}

BOOL FixedScheduleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateDialog();
	m_isOnlyAvailableCtrl = m_fixedSchedule.IsOnlyAvailable();
	UpdateData(FALSE);
	
	CComboBox * pBox = (CComboBox*)GetDlgItem(IDC_DAY_NAME);
	pBox->ResetContent();
	for (int i=0;i<7;i++)
	{
		pBox->AddString(WeekName(i));
	}
	pBox->SetCurSel(0);

	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_name);

	return TRUE;
}

void FixedScheduleDlg::OnBnClickedAdd()
{
	try
	{
		UpdateData(TRUE);
		m_fixedSchedule.Add(FixedScheduleEntry(m_weekNumberCtrl,
			m_dayNumberCtrl,m_startTime,m_hours));
	}
	MY_CATCH()
	UpdateDialog();

}

void FixedScheduleDlg::OnBnClickedRemove()
{
	int sel = m_fixedListCtrl.GetCurSel();
	if (sel == LB_ERR)
		return;
	m_fixedSchedule.Remove(sel);
	UpdateDialog();
	if (sel == m_fixedListCtrl.GetCount())
	{
		sel--;
		if (sel == -1)
			return;
	}
	m_fixedListCtrl.SetCurSel(sel);
}

void FixedScheduleDlg::OnBnClickedChange()
{
	UpdateData(TRUE);
	int sel = m_fixedListCtrl.GetCurSel();
	if (sel == LB_ERR)
		return;
	try
	{
		FixedSchedule tempSchedule(m_fixedSchedule);
		tempSchedule.Remove(sel);
		tempSchedule.Add(FixedScheduleEntry(m_weekNumberCtrl,
				m_dayNumberCtrl,m_startTime,m_hours));
		m_fixedSchedule = tempSchedule;
	}
	MY_CATCH()
	UpdateDialog();
	m_fixedListCtrl.SetCurSel(sel);
}

void FixedScheduleDlg::OnLbnSelchangeFixedList()
{
	int currentlySelectedItem = m_fixedListCtrl.GetCurSel();
	if (currentlySelectedItem != -1)
	{
			const FixedScheduleEntry&
				entry = m_fixedSchedule[currentlySelectedItem];
			this->m_weekNumberCtrl = entry.Week();
			this->m_dayNumberCtrl = entry.Day();
			this->m_dayNameCtrl = entry.Day();
			this->m_hours = entry.Hours();
			this->m_startTime = entry.StartTime();
	}
	UpdateData(FALSE);
}

void FixedScheduleDlg::OnEnKillfocusDayNumber()
{
	UpdateData(TRUE);
	if ((m_dayNumberCtrl >= 0) && m_dayNumberCtrl < 7)
	{
		m_dayNameCtrl = m_dayNumberCtrl;
	}
	UpdateData(FALSE);
	
}

void FixedScheduleDlg::OnCbnSelchangeDayName()
{
	UpdateData(TRUE);
	m_dayNumberCtrl = m_dayNameCtrl;
	UpdateData(FALSE);
}

void FixedScheduleDlg::OnOK()
{
	UpdateData(TRUE);
	m_fixedSchedule.SetOnlyAvailable(this->m_isOnlyAvailableCtrl);

	CDialog::OnOK();
}
