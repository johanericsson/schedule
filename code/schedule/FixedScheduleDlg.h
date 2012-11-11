#pragma once
#include "resource.h"
#include <vector>
#include "FixedSchedule.h"
#include "afxwin.h"

// FixedScheduleDlg dialog

class FixedScheduleDlg : public CDialog
{
	DECLARE_DYNAMIC(FixedScheduleDlg)

public:
	FixedScheduleDlg(CString name,FixedSchedule fixedSchedule,
		int startTime,int hours);   
	void UpdateDialog();
	FixedSchedule m_fixedSchedule;
// Dialog Data
	enum { IDD = IDD_FIXED_SCHEDULE };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_name;
	DECLARE_MESSAGE_MAP()
public:
	int m_weekNumberCtrl;
	int m_dayNumberCtrl;
	int m_startTime;
	int m_hours;
	virtual BOOL OnInitDialog();
	CListBox m_fixedListCtrl;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	int m_dayNameCtrl;
	afx_msg void OnBnClickedChange();
	afx_msg void OnLbnSelchangeFixedList();
	afx_msg void OnEnKillfocusDayNumber();
	afx_msg void OnCbnSelchangeDayName();
	BOOL m_isOnlyAvailableCtrl;
protected:
	virtual void OnOK();
};
