// EditNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "EditNameDlg.h"


// EditNameDlg dialog

EditNameDlg::EditNameDlg(const CString& oldName)
	: CDialog(EditNameDlg::IDD, NULL)
	, m_oldName(oldName)
	, m_newName(_T(""))
{
}


void EditNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OLD_NAME, m_oldName);
	DDX_Text(pDX, IDC_NEW_NAME, m_newName);
}


BEGIN_MESSAGE_MAP(EditNameDlg, CDialog)
END_MESSAGE_MAP()


// EditNameDlg message handlers
