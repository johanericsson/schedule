#pragma once
#include "afxwin.h"
#include "Shifts.h"
#include "resource.h"


// ShiftsTemplateDlg dialog
class ShiftsTemplateDlg : public CDialog
{
	DECLARE_DYNAMIC(ShiftsTemplateDlg)
	ShiftsTemplate& m_shifts;
public:
	void UpdateDialog();
	ShiftsTemplateDlg(ShiftsTemplate&);   // standard constructor
	virtual ~ShiftsTemplateDlg();

// Dialog Data
	enum { IDD = IDD_SHIFTS_TEMPLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_weekDayScheduleCtrl;
	virtual BOOL OnInitDialog();
	int m_weekDay;
	int m_startTime;
	int m_number;
	afx_msg void OnCbnSelchangeWeekday();
	
	void OnBnClickedAdd();
	void OnBnClickedRemove();
	int m_hours;
};