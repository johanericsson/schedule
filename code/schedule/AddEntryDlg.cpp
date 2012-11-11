// AddEntryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "AddEntryDlg.h"
#include ".\addentrydlg.h"


// AddEntryDlg dialog

IMPLEMENT_DYNAMIC(AddEntryDlg, CDialog)
AddEntryDlg::AddEntryDlg(const Nurse& nurse)
	: CDialog(AddEntryDlg::IDD, NULL)
	, m_startTime(nurse.GetStartTime())
	, m_numberOfHours(nurse.GetDailyHours())
	, m_name(nurse.GetName().FullName())
{
}

void AddEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_START_TIME, m_startTime);
	DDX_Text(pDX, IDC_NUMBER_OF_HOURS, m_numberOfHours);
	DDX_Text(pDX, IDC_STATIC_NAME, m_name);
}


BEGIN_MESSAGE_MAP(AddEntryDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// AddEntryDlg message handlers

void AddEntryDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_startTime < 0 || m_startTime >=24)
		return;
	if (m_numberOfHours <= 0 || m_numberOfHours >=24)
		return;
	OnOK();
}
