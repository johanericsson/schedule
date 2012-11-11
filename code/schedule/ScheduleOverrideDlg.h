#pragma once
#include "afxwin.h"
#include "ScheduleAssignments.h"


// ScheduleOverrideDlg dialog

class ScheduleOverrideDlg : public CDialog
{
public:
	ScheduleOverrideDlg(int dayNumber);   // standard constructor


	vector<TemporarySchedule> m_schedules;
	int m_dayNumber;
// Dialog Data
	enum { IDD = IDD_SCHEDULE_OVERRIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_overridesListCtrl;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	int m_startTimeCtrl;
	int m_numberOfSlots;

	afx_msg void OnBnClickedOk();
	CString m_dayName;
};
