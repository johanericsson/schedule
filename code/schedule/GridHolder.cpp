// GridHolder.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "GridHolder.h"


// GridHolder dialog

IMPLEMENT_DYNAMIC(GridHolder, CDialog)
GridHolder::GridHolder(CWnd* pParent /*=NULL*/)
	: CDialog(GridHolder::IDD, pParent)
{
}

GridHolder::~GridHolder()
{
}

void GridHolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GridHolder, CDialog)
END_MESSAGE_MAP()


// GridHolder message handlers

BOOL GridHolder::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
