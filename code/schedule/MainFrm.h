// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "scheduleview.h"
class CMainFrame : public CMDIFrameWnd
{
	afx_msg void OnPaint();
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	void OnSize(UINT,int,int);
// Attributes
public:

// Operations
public:
	ScheduleView *m_scheduleView;
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL CreateClient(LPCREATESTRUCT lpCreateStruct, CMenu* pWindowMenu);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnPrint();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};


