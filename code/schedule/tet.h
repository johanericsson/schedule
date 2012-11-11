#pragma once
#include "afxdtctl.h"
#include "afxwin.h"


// tet dialog

class tet : public CDialog
{
	DECLARE_DYNAMIC(tet)

public:
	tet(CWnd* pParent = NULL);   // standard constructor
	virtual ~tet();

// Dialog Data
	enum { IDD = IDD_DAILY_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMcnSelchangeCalendar(NMHDR *pNMHDR, LRESULT *pResult);
	CMonthCalCtrl m_Calendar;
	CComboBox m_Test;
};
