#pragma once

class LeadNurseView : public CScrollView
{
public:
	void OnDraw(CDC *pDC);
	DECLARE_DYNCREATE(LeadNurseView);
	void OnInitialUpdate();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	CTime m_date;
	LeadNurseView();
	CString GetDayName() const
	{
		return m_date.Format(_T("%A")); // return weekday name
	}
	CString GetDate() const
	{
		return m_date.Format(_T("%b %d, %Y"));
	}
	
	void OnPreviousDay();
	void OnNextDay();
	void OnNextWeek();
	void OnPreviousWeek();
	void OnPrintOne();
	void OnPrintall();
	void OnPrintWeek();
};