#pragma once


// ScheduleSignup dialog

class ScheduleSignup : public CDialog
{

public:
	vector<vector<CComboBox*> > m_shifts;
	vector<CStatic*> m_shiftTitles;
	ScheduleSignup(Data& );   // standard constructor
	~ScheduleSignup() {FreeAll();}
	void UpdateAll();
	void FreeAll();

	Data& m_data;
// Dialog Data
	enum { IDD = IDD_DAILY_SCHEDULE };

protected:
	void OnMcnSelchangeCalendar(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CMonthCalCtrl m_Calendar;
	CComboBox m_Test;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
