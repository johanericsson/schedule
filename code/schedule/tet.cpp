// tet.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "tet.h"


// tet dialog

IMPLEMENT_DYNAMIC(tet, CDialog)
tet::tet(CWnd* pParent /*=NULL*/)
	: CDialog(tet::IDD, pParent)
{
}

tet::~tet()
{
}

void tet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CALENDAR, m_Calendar);
	DDX_Control(pDX, IDC_TEST, m_Test);
}


BEGIN_MESSAGE_MAP(tet, CDialog)
	ON_NOTIFY(MCN_SELCHANGE, IDC_CALENDAR, OnMcnSelchangeCalendar)
END_MESSAGE_MAP()


// tet message handlers

void tet::OnMcnSelchangeCalendar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
