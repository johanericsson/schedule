// LegendEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LegendEditorDlg.h"
#include "MyException.h"

// LegendEditorDlg dialog

IMPLEMENT_DYNAMIC(LegendEditorDlg, CDialog)
LegendEditorDlg::LegendEditorDlg(ShiftLegend& legend)
	: CDialog(LegendEditorDlg::IDD),
	m_legend(legend),
	m_oldLegend(legend)
	, m_abbreviationNameCtrl(_T(""))
	, m_startTimeCtrl(0)
	, m_numberOfHoursCtrl(0)
{
}

void LegendEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ABBREVIATION_NAME, m_abbreviationNameCtrl);
	DDX_Text(pDX, IDC_START_TIME, m_startTimeCtrl);
	DDX_Text(pDX, IDC_NUMBER_OF_HOURS, m_numberOfHoursCtrl);
	DDX_Control(pDX, IDC_ABBREVIATION_LIST, m_listCtrl);
}


BEGIN_MESSAGE_MAP(LegendEditorDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_CHANGE, OnBnClickedChange)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_ABBREVIATION_LIST, OnLbnSelchangeAbbreviationList)
END_MESSAGE_MAP()

void LegendEditorDlg::UpdateList()
{
	m_listCtrl.ResetContent();
	for (int i=0;i<m_legend.Size();i++)
	{
		m_listCtrl.AddString(ToString(m_legend[i]));
	}
	UpdateData(FALSE);
}
void LegendEditorDlg::OnBnClickedAdd()
{
	try
	{
		UpdateData(TRUE);
		m_legend.AddAbbreviation(
			Abbreviation(m_startTimeCtrl,
			m_numberOfHoursCtrl,m_abbreviationNameCtrl));
		UpdateList();
	}
	MY_CATCH()
}

void LegendEditorDlg::OnBnClickedChange()
{
	UpdateData(TRUE);
	int sel = m_listCtrl.GetCurSel();
	if (sel == LB_ERR)
		return;
	m_legend.Remove(sel);
	m_legend.AddAbbreviation(
			Abbreviation(m_startTimeCtrl,
			m_numberOfHoursCtrl,m_abbreviationNameCtrl));

	UpdateList();
}

void LegendEditorDlg::OnBnClickedRemove()
{
	UpdateData(TRUE);
	int sel = m_listCtrl.GetCurSel();
	if (sel == LB_ERR)
		return;
	m_legend.Remove(sel);
	UpdateList();
	
}
void LegendEditorDlg::OnBnClickedOk()
{
	m_oldLegend = m_legend;	
	OnOK();
}

BOOL LegendEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LegendEditorDlg::OnLbnSelchangeAbbreviationList()
{
	UpdateData(TRUE);
	int sel = m_listCtrl.GetCurSel();
	if (sel == LB_ERR)
		return;

	m_abbreviationNameCtrl = m_legend[sel].abbreviation;
	m_startTimeCtrl = m_legend[sel].startTime;
	m_numberOfHoursCtrl = m_legend[sel].hours;
	UpdateData(FALSE);
}
