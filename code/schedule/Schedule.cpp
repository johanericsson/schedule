// Schedule.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Schedule.h"
#include "MainFrm.h"
#include "Gridholder.h"
#include "DateRangeDlg.h"
#include "LegendEditorDlg.h"
#include "dailyassignmentsdlg.h"
#include "scheduledoc.h"
#include "childfrm.h"
#include "signupview.h"
#include "Scheduleviewex.h"
#include ".\schedule.h"
#include "weekview.h"
#include "DailyOverview.h"
#include "needsview.h"
#include <fstream>
#include "XMLNode.h"
#include "VacationTypeDlg.h"
#include "leadnurseview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScheduleApp

BEGIN_MESSAGE_MAP(CScheduleApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_SAVE,OnSave)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_VIEW_NAMELIST, OnViewNamelist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAMELIST, OnUpdateViewNamelist)
	ON_COMMAND(ID_VIEW_SCHEDULESIGNUP, OnViewScheduleSignup_)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCHEDULESIGNUP, OnUpdateViewScheduleSignup)
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_COMMAND(ID_SHIFT_TEMPLATE,OnViewTemplate)
	ON_UPDATE_COMMAND_UI(ID_SHIFT_TEMPLATE,OnUpdateViewTemplate)
	ON_COMMAND(ID_LEGEND,OnViewLegendEditor)
	ON_COMMAND(ID_ASSIGNMENTS,OnViewDailyAssignments)
	ON_COMMAND(ID_VIEW_SCHEDULESIGNUP,OnViewScheduleSignup_)
	ON_COMMAND(ID_VIEW_SCHEDULE, OnViewPagedScheduleView)
	ON_COMMAND(ID_VIEW_PAGED_SCHEDULE_VIEW,OnViewPagedScheduleView)
	ON_COMMAND(ID_WEEKVIEW, OnWeekview)
	ON_COMMAND(ID_VIEW_DAILYOVERVIEW, OnViewDailyoverview)
	ON_COMMAND(ID_VIEW_NEEDSVIEW, OnViewNeedsview)
	ON_COMMAND(ID_VIEWVACATIONTYPES, OnViewvacationtypes)
	ON_COMMAND(ID_LEADNURSEBUTTON,OnViewLeadNurse)
END_MESSAGE_MAP()


// CScheduleApp construction

CScheduleApp::CScheduleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

BOOL Is_Win2000_SP1_or_Later () 
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osvi.dwMajorVersion = 5;
	osvi.dwMinorVersion = 0;
	osvi.wServicePackMajor = 1;

	// Initialize the condition mask.

	VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, 
		VER_GREATER_EQUAL );
	VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, 
		VER_GREATER_EQUAL );
	VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, 
		VER_GREATER_EQUAL );

	// Perform the test.

	return VerifyVersionInfo(
		&osvi, 
		VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR,
		dwlConditionMask);
}


// The one and only CScheduleApp object

CScheduleApp theApp;

// CScheduleApp initialization

// If InitInstance returns false, then 
// AfxMessageBox doesn't work in exitInstance. Anyways, we
// know that we don't want to save changes
static bool g_goingDown = false;
BOOL CScheduleApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	
	
	m_pScheduleView = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CScheduleDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(ScheduleView));
	AddDocTemplate(m_pScheduleView);

	m_pScheduleSignup= new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CScheduleDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(SignupView));
	AddDocTemplate(m_pScheduleSignup);

	m_pScheduleViewEx = 
		new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CScheduleDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(ScheduleViewEx));
	AddDocTemplate(m_pScheduleViewEx);

	m_pWeekView = 
		new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CScheduleDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(WeekView));
	AddDocTemplate(m_pWeekView);

	m_pDailyOverview= 
		new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CScheduleDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(DailyOverview));
	AddDocTemplate(m_pDailyOverview);

	m_pNeedsView =
		new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CScheduleDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(NeedsView));
	AddDocTemplate(m_pNeedsView);

	m_pLeadNurseView =
		new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CScheduleDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(LeadNurseView));
	AddDocTemplate(m_pLeadNurseView);



	// create main MDI Frame window
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	BOOL isGoodOS = Is_Win2000_SP1_or_Later();
	if (!isGoodOS)
	{
		g_goingDown = true;
		AfxMessageBox(_T("This application requires Windows XP or later"));
		return FALSE;
	}
	try
	{
		ifstream file("database.xml");
		XMLNode node;
		file >> node;
		node >> m_periods;
	}
	catch(const OurException& ex)
	{
		AfxMessageBox(ex.m_str);
	}
	catch (CFileException * pException)
	{
		if (pException->m_cause != 
			CFileException::fileNotFound)
		{
			// notnormal
			pException->ReportError();
		}
		m_periods= Periods();
	}
	catch (CException * pException)
	{
		pException->ReportError();
		m_periods = Periods();
	}


	if (m_periods.GetCurrent() == -1)
	{
		DateRangeDlg dlg(m_periods);
		int res = (int)dlg.DoModal();
		if (res != IDOK)
		{
			g_goingDown = true;
			return FALSE;
		}
	}

	CFrameWnd* frame = m_pScheduleSignup->CreateNewFrame(
		((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument()
		,NULL);
	frame->ShowWindow(SW_SHOW);
	UpdateAll();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CScheduleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CScheduleApp message handlers
template <class T,class Initializer>
static void ToggleDialog(T& pT,UINT resourceID,Initializer& initializer)
{
	if (!pT.get())
	{
		pT = T(new T::element_type(initializer));
		BOOL success = pT->Create(resourceID);
		ASSERT(success);
	}
	pT->ShowWindow(!pT->IsWindowVisible());
}

void CScheduleApp::OnViewNamelist()
{
	
	ToggleDialog(m_nameDlg,IDD_NAMES,m_periods.GetCurrentPeriod().m_data.m_nurses);	
}

void CScheduleApp::OnUpdateViewNamelist(CCmdUI *pCmdUI)
{
	if (m_nameDlg.get())
	{
		if (m_nameDlg->IsWindowVisible())
		{
			pCmdUI->SetCheck();
			return;
		}
	}
	pCmdUI->SetCheck(FALSE);
}
void CScheduleApp::OnViewScheduleSignup()
{
	ToggleDialog(m_signup,IDD_DAILY_SCHEDULE,m_periods.GetCurrentPeriod().m_data);
}
void CScheduleApp::OnUpdateViewScheduleSignup(CCmdUI * pCmdUI)
{
	if (m_signup.get())
	{
		if (m_signup->IsWindowVisible())
		{
			pCmdUI->SetCheck();
			return;
		}
	}
	pCmdUI->SetCheck(FALSE);
	
}

int CScheduleApp::ExitInstance()
{
	if (!g_goingDown)
	{
		int ret = AfxMessageBox(_T("Would you like to save your changes?"),MB_YESNO);
		if (IDYES== ret)
			OnSave();
	}
	
	return CWinApp::ExitInstance();
}

void CScheduleApp::OnViewTemplate()
{
	ToggleDialog(m_shiftsTemplateDlg,IDD_SHIFTS_TEMPLATE,
		m_periods.GetCurrentPeriod().m_data.m_ShiftsTemplate);
}

void CScheduleApp::OnUpdateViewTemplate(CCmdUI * pCmdUI)
{
	if (m_shiftsTemplateDlg.get())
	{
		if (m_shiftsTemplateDlg->IsWindowVisible())
		{
			pCmdUI->SetCheck();
			return;
		}
	}
	pCmdUI->SetCheck(FALSE);
}

void CScheduleApp::OnViewGrid()
{
	GridHolder h;
	h.DoModal();
}

void CScheduleApp::OnViewLegendEditor()
{
	LegendEditorDlg dlg(m_periods.GetCurrentPeriod().m_data.m_legend);
	dlg.DoModal();
	UpdateAll();
}

void CScheduleApp::OnViewDailyAssignments()
{
	DailyAssignmentsDlg dlg(m_periods.GetCurrentPeriod());
	dlg.DoModal();
	UpdateAll();
}

CFrameWnd* CreateNewView(CMultiDocTemplate* docTemplate)
{
	CFrameWnd * frame = docTemplate->CreateNewFrame(((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument()
		,NULL);
	frame->ShowWindow(SW_SHOW);
	CRect rcWnd;
	frame->GetWindowRect(&rcWnd);
	AfxGetMainWnd()->ScreenToClient(&rcWnd);
	frame->SetWindowPos(&CWnd::wndTop,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),0);
	return frame;
	
}

void CScheduleApp::OnViewScheduleSignup_()
{
	CreateNewView(m_pScheduleSignup);
}
void CScheduleApp::OnViewSchedule()
{
	CreateNewView(m_pScheduleView);
}

void CScheduleApp::OnViewPagedScheduleView()
{
	CreateNewView(m_pScheduleViewEx);
}

void CScheduleApp::OnWeekview()
{
	CreateNewView(m_pWeekView);
}

void CScheduleApp::OnViewDailyoverview()
{
	CreateNewView(m_pDailyOverview);
}

void CScheduleApp::OnViewNeedsview()
{
	CreateNewView(m_pNeedsView);
}

void CScheduleApp::OnViewLeadNurse()
{
	CreateNewView(m_pLeadNurseView);
}

void CScheduleApp::OnViewvacationtypes()
{
	VacationTypeDlg dlg(m_periods.GetCurrentPeriod().m_data.m_vacationTypes);
	if (IDOK==dlg.DoModal())
		UpdateAll();
}

void CScheduleApp::OnSave()
{
		try
	{
		XMLNode topLevel(_T("ScheduleData"));
		topLevel << m_periods;

		std::ofstream fOut("database.xml");
		fOut << topLevel;
	}
	catch (CFileException * pException)
	{
		AfxMessageBox(pException->ReportError());
	}
}
