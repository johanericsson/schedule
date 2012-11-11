// VacationTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "VacationTypeDlg.h"
#include ".\vacationtypedlg.h"


// VacationTypeDlg dialog

VacationTypeDlg::VacationTypeDlg(VacationTypes& types)
	: CDialog(VacationTypeDlg::IDD, NULL),
	m_code(_T("")),
	m_description(_T("")),
	m_types(types),
	m_oldTypes(types)
{
}

void VacationTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CODE, m_code);
	DDX_Text(pDX, IDC_DESCRIPTION, m_description);
	DDX_Control(pDX, IDC_TYPES, m_typesCtrl);
}


BEGIN_MESSAGE_MAP(VacationTypeDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// VacationTypeDlg message handlers

void VacationTypeDlg::OnBnClickedAdd()
{
	UpdateData(TRUE);
	if (m_code.GetLength() ==0)
		return;
	VacationType type;
	type.m_code = m_code;
	type.m_description = m_description;
	m_types.m_types.push_back(type);
	sort(m_types.m_types.begin(),m_types.m_types.end());
	UpdateListBox();
}

void VacationTypeDlg::OnBnClickedRemove()
{
	int sel = m_typesCtrl.GetCurSel();
	if (sel == LB_ERR)
		return;
	m_types.m_types.erase(m_types.m_types.begin() + sel);
	UpdateListBox();
}

void VacationTypeDlg::OnBnClickedOk()
{
	m_oldTypes = m_types;
	OnOK();
}

void VacationTypeDlg::UpdateListBox()
{
	m_typesCtrl.ResetContent();
	for (int i=0;i<m_types.m_types.size();i++)
	{
		m_typesCtrl.AddString(ToString(m_types.m_types[i]));
	}
}

BOOL VacationTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateListBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
