// DateRangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DateRangeDlg.h"
#include "updater.h"
#include "optional.h"

// DateRangeDlg dialog

IMPLEMENT_DYNAMIC(DateRangeDlg, CDialog)
DateRangeDlg::DateRangeDlg(Periods& periods,CWnd* pParent /*=NULL*/)
	: CDialog(DateRangeDlg::IDD, pParent), 
	m_numberOfWeeks(0),
	m_periods(periods),
	m_originalPeriods(periods)
{
}

void DateRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_DATE, m_startDateCtrl);
	DDX_Text(pDX, IDC_NUMBER_OF_WEEKS, m_numberOfWeeks);
	DDX_Control(pDX, IDC_END_DATE, m_endDateCtrl);
	DDX_Control(pDX, IDC_DATE_RANGES, m_dateRangesCtrl);
}


BEGIN_MESSAGE_MAP(DateRangeDlg, CDialog)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_NOTIFY(MCN_SELCHANGE, IDC_START_DATE, OnMcnSelchangeStartDate)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_DATE_RANGES, OnLbnSelchangeDateRanges)
END_MESSAGE_MAP()


void DateRangeDlg::UpdateDialog()
{
	UpdateData(TRUE);
	m_dateRangesCtrl.ResetContent();
	for (int i = 0;i<m_periods.Size();i++)
	{
		m_dateRangesCtrl.AddString(m_periods[i].m_dateRange.ToString());
	}
	m_dateRangesCtrl.SetCurSel(m_periods.GetCurrent());

	CTime startDate;
	BOOL success = m_startDateCtrl.GetCurSel(startDate);
	ASSERT(success);
	CTimeSpan timeSpan(m_numberOfWeeks * 7,0,0,0);
	CTime endDate = startDate + timeSpan;
	success = m_endDateCtrl.SetCurSel(endDate);
	int curSel = m_dateRangesCtrl.GetCurSel();
	if (curSel == LB_ERR)
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	else
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	UpdateData(FALSE);
	UpdateAll();
}

void DateRangeDlg::OnBnClickedApply()
{
	UpdateDialog();
}

BOOL DateRangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_periods.Size())
		m_periods.SetCurrent(m_periods.Size() - 1);
	m_numberOfWeeks = 6;
	UpdateDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DateRangeDlg::OnMcnSelchangeStartDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateDialog();
}

void DateRangeDlg::OnBnClickedAdd()
{
	UpdateData(TRUE);
	CTime currentSelection;
	BOOL success = m_startDateCtrl.GetCurSel(currentSelection);
	ASSERT(success);
	// Need to clear other fields of the date.
	// stupid month control returns garbage for hours/seconds/minutes
	currentSelection = 
		CTime(currentSelection.GetYear(),currentSelection.GetMonth(),
		      currentSelection.GetDay(),0,0,0,0);
						
	int curSel = m_dateRangesCtrl.GetCurSel();
	Optional<Period> oldPeriod;
	if (curSel != LB_ERR)
		oldPeriod = m_periods[curSel];
	Period newPeriod(DateRange(currentSelection,m_numberOfWeeks));
	if (oldPeriod)
		newPeriod.Import(*oldPeriod);
	m_periods.Add(newPeriod);


	UpdateDialog();	
}

void DateRangeDlg::OnBnClickedRemove()
{
	UpdateData(TRUE);
	int curSel = m_dateRangesCtrl.GetCurSel();
	if (curSel == LB_ERR)
		return;
	m_periods.Remove(curSel);
	UpdateDialog();
}

void DateRangeDlg::OnBnClickedOk()
{
	m_originalPeriods= m_periods;	
	OnOK();
}

void DateRangeDlg::OnLbnSelchangeDateRanges()
{
	UpdateData(TRUE);
	int curSel = m_dateRangesCtrl.GetCurSel();
	if (curSel == LB_ERR)
		return;
	m_periods.SetCurrent(curSel);
	UpdateDialog();
}
