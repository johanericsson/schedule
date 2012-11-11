#pragma once

#include "resource.h"
#include "Period.h"
#include "afxwin.h"
#include "afxdtctl.h"
class DailyAssignmentsDlg : public CDialog
{

public:
	DailyAssignmentsDlg(Period& period);   // standard constructor
	DailyAssignmentsDlg(Period& period,int date);
	int m_date;
	Period& m_oldPeriod;
	Period m_period;
// Dialog Data
	enum { IDD = IDD_DAILY_ASSIGNMENTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	void UpdateFixed();
	void UpdateControls();
	CListBox m_nameListCtrl;
	CListBox m_scheduleListCtrl;
	CMonthCalCtrl m_dateCtrl;
	afx_msg void OnMcnSelchangeDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedChange();
	afx_msg void OnBnClickedRemove();
	int m_startTimeCtrl;
	afx_msg void OnLbnSelchangeNameList();
	int m_numberOfHoursCtrl;
	bool m_isOverrideOn;
	CListBox m_overrideList;
	afx_msg void OnBnClickedEnableFixed();
	afx_msg void OnBnClickedEditVacations();
};
