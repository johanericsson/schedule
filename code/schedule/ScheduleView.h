// ScheduleView.h : interface of the CScheduleView class
//


#pragma once
#include "Updater.h"
#include "optional.h"

CString ViewOutput(CTime firstDay,CTime lastDay,
				   int firstName,int lastName);
CString ViewAllOutput();
class ScheduleView : public CScrollView,
					 public UpdateSubject
{
public:

	DECLARE_DYNCREATE(ScheduleView)
	Optional<CString> m_strOutput;
	DECLARE_MESSAGE_MAP()
	void OnInitialUpdate();
	void OnDraw(CDC *pDC);
	CSize m_oldSize;
public:
	ScheduleView()
	{
	}
	
	virtual void UpdateTheSubject();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};