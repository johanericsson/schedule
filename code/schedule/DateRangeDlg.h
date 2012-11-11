#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "period.h"
#include "resource.h"


// DateRangeDlg dialog

class DateRangeDlg : public CDialog
{
	DECLARE_DYNAMIC(DateRangeDlg)

public:
	DateRangeDlg(Periods& periods,
		CWnd* pParent = NULL);   // standard constructor

	Periods& m_originalPeriods;
	Periods m_periods;
	
	void UpdateDialog();
// Dialog Data
	enum { IDD = IDD_DATE_RANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMonthCalCtrl m_startDateCtrl;
	long m_numberOfWeeks;
	CMonthCalCtrl m_endDateCtrl;
	afx_msg void OnBnClickedApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnMcnSelchangeStartDate(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_dateRangesCtrl;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeDateRanges();
};
