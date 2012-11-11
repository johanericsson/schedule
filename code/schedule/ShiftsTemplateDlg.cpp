// ShiftsTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShiftsTemplateDlg.h"
#include "shifts.h"


// ShiftsTemplateDlg dialog

IMPLEMENT_DYNAMIC(ShiftsTemplateDlg, CDialog)
ShiftsTemplateDlg::ShiftsTemplateDlg(ShiftsTemplate& shifts)
	: CDialog(ShiftsTemplateDlg::IDD, NULL),//pParent),
	m_shifts(shifts)
	, m_weekDay(0)
	, m_startTime(0)
	, m_number(0)
	, m_hours(0)
{
}

ShiftsTemplateDlg::~ShiftsTemplateDlg()
{
}

void ShiftsTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEEK_DAY_SCHEDULE, m_weekDayScheduleCtrl);
	DDX_CBIndex(pDX, IDC_WEEKDAY, m_weekDay);
	DDX_Text(pDX, IDC_START_TIME, m_startTime);
	DDX_Text(pDX, IDC_NUMBER, m_number);
	DDX_Text(pDX, IDC_NUMBER_OF_HOURS, m_hours);
}


BEGIN_MESSAGE_MAP(ShiftsTemplateDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_WEEKDAY, OnCbnSelchangeWeekday)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
END_MESSAGE_MAP()

void ShiftsTemplateDlg::UpdateDialog()
{
	UpdateData(TRUE);
	m_weekDayScheduleCtrl.ResetContent();
	for (int i =0;i<m_shifts[m_weekDay].Size();i++)
	{
		m_weekDayScheduleCtrl.
			AddString(ToString(m_shifts[m_weekDay][i]));
	}
}

BOOL ShiftsTemplateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_weekDay = 0;
	CComboBox * pC = (CComboBox*)GetDlgItem(IDC_WEEKDAY);
	int i;
	pC->ResetContent();
	for (i=0;i<7;i++)
	{
		pC->AddString(m_shifts[i].GetWeekName());
	}

	UpdateData(FALSE);

	UpdateDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ShiftsTemplateDlg::OnCbnSelchangeWeekday()
{
	UpdateDialog();
}

void ShiftsTemplateDlg::OnBnClickedAdd()
{
	UpdateData(TRUE);
	m_shifts[m_weekDay].AddTemplate(m_startTime,m_number,m_hours);
	UpdateDialog();
}

void ShiftsTemplateDlg::OnBnClickedRemove()
{
	UpdateData(TRUE);
	int i = m_weekDayScheduleCtrl.GetCurSel();
	if (i==LB_ERR)
		return;
	m_shifts[m_weekDay].Remove(i);
	UpdateDialog();
}