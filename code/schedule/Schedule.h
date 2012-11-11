// Schedule.h : main header file for the Schedule application
//
#pragma once

#include "resource.h"       // main symbols
#include "Period.h"
#include "NameDlg.h"
#include "ScheduleSignup.h"
#include "ShiftsTemplateDlg.h"
#include <memory>
// CScheduleApp:
// See Schedule.cpp for the implementation of this class
//

class CScheduleApp : public CWinApp
{
public:
	void OnViewScheduleSignup_();
	auto_ptr<ShiftsTemplateDlg> m_shiftsTemplateDlg;
	auto_ptr<ScheduleSignup> m_signup;
	auto_ptr<NameDlg> m_nameDlg;
	Periods m_periods;
	
	CScheduleApp();

	CMultiDocTemplate* m_pScheduleView;
	CMultiDocTemplate* m_pScheduleSignup;
	CMultiDocTemplate* m_pScheduleViewEx;
	CMultiDocTemplate* m_pWeekView;
	CMultiDocTemplate* m_pDailyOverview;
	CMultiDocTemplate* m_pNeedsView;
	CMultiDocTemplate* m_pLeadNurseView;



// Overrides
public:
	virtual BOOL InitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewNamelist();
	afx_msg void OnUpdateViewNamelist(CCmdUI *pCmdUI);
	afx_msg void OnViewScheduleSignup();
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewScheduleSignup(CCmdUI * pCmdUI);
	afx_msg void OnViewTemplate();
	afx_msg void OnUpdateViewTemplate(CCmdUI * pCmdUI);
	virtual int ExitInstance();
	afx_msg void OnViewLegendEditor();
	void OnViewDailyAssignments();
	afx_msg void OnViewSchedule();
	void OnViewPagedScheduleView();
	afx_msg void OnWeekview();
	afx_msg void OnViewDailyoverview();
	afx_msg void OnViewNeedsview();
	afx_msg void OnViewvacationtypes();
	afx_msg void OnSave();
	afx_msg void OnViewLeadNurse();
};

CFrameWnd* CreateNewView(CMultiDocTemplate* docTemplate);
inline Period& GetPeriod()
{
	return ((CScheduleApp*)AfxGetApp())->m_periods.GetCurrentPeriod();
}
extern CScheduleApp theApp;