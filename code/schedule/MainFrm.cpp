// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Schedule.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_PRINT,OnPrint)
END_MESSAGE_MAP()

void CMainFrame::OnPrint()
{
	CString strOut = ViewAllOutput();
	VERIFY(OpenClipboard());
	VERIFY(EmptyClipboard());
	HGLOBAL alloc = GlobalAlloc(GMEM_MOVEABLE,strOut.GetLength()+1);
	char * ptr = (char*)GlobalLock(alloc);
	memcpy(ptr,strOut.GetBuffer(0),strOut.GetLength()+1);
	GlobalUnlock(alloc);
	VERIFY(SetClipboardData(CF_TEXT,alloc));
	VERIFY(CloseClipboard());

}

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}
static CRect GetRc(const CWnd * pWnd,const CWnd * topWnd,
				   const CWnd * bottomWnd)
{
	CRect rcClient;
	pWnd->GetClientRect(&rcClient);
	CRect rcTop;
	topWnd->GetWindowRect(&rcTop);
	CRect rcBottom;
	bottomWnd->GetWindowRect(&rcBottom);
	int top = rcTop.Height();
	int bottom = rcBottom.Height();
	CRect offset(0,-top+4,0,-bottom);
	
	return rcClient + offset;
}

void CMainFrame::OnSize(UINT nType,int cx,int cy)
{
	CMDIFrameWnd::OnSize(nType,cx,cy);

//	m_scheduleView->MoveWindow(GetRc(this,&m_wndToolBar,
								//&m_wndStatusBar));
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	m_scheduleView = new ScheduleView;
	//VERIFY(m_scheduleView->Create(NULL, NULL, WS_HSCROLL | WS_VSCROLL | 
		//AFX_WS_DEFAULT_VIEW,
		//CRect(0, 0, 0, 0), this, 1234, NULL));

	

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

//	m_scheduleView->MoveWindow(GetRc(this,&m_wndToolBar,
								//&m_wndStatusBar));
	//m_scheduleView->ShowWindow(SW_SHOW);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{

	AfxMessageBox(_T("Hi "));
	CMDIFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	CMDIFrameWnd::OnMouseMove(nFlags, point);
}

BOOL CMainFrame::CreateClient(LPCREATESTRUCT lpCreateStruct, CMenu* pWindowMenu)
{
	return CMDIFrameWnd::CreateClient(lpCreateStruct, pWindowMenu);
}

void CMainFrame::OnPaint()
{
/*	CClientDC dc(this);
	CRect rcView;
	GetClientRect(&rcView);
	dc.FillRect(rcView,&CBrush(RGB(0,255,0)));
*/	CMDIFrameWnd::OnPaint();
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
/*		CClientDC dc(this);
	CRect rcView;
	GetClientRect(&rcView);
	dc.FillRect(rcView,&CBrush(RGB(0,255,0)));
*/
	
	return CMDIFrameWnd::OnEraseBkgnd(pDC);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//if (m_scheduleView)
	//{
		//if (m_scheduleView->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
			//return TRUE;
//
	//}

	return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
