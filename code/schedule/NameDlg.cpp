// NameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NameDlg.h"
#include "MyException.h"
#include "Helpers.h"
#include "nurse.h"
#include "FixedScheduleDlg.h"
#include "updater.h"
#include "EditNameDlg.H"

// NameDlg

NameDlg::NameDlg(Nurses& nurses)
	: CDialog(NameDlg::IDD),
	m_nurses(nurses)
{
}

void NameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_NAME, m_newName);
}

BEGIN_MESSAGE_MAP(NameDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(ID_CLOSE, OnBnClickedClose)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDC_EDIT_FIXED, OnBnClickedEditFixed)
	ON_BN_CLICKED(IDC_EDIT_NAME, OnBnClickedEditName)
END_MESSAGE_MAP()


// NameDlg diagnostics

#ifdef _DEBUG
void NameDlg::AssertValid() const
{
	CDialog::AssertValid();
}

void NameDlg::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG

vector<vector<CString> > GetStrings(const Nurses& n)
{
	vector<vector<CString> > names(n.Size(),vector<CString>(2));
	for (int i = 0;
 		 i < n.Size();
		 i++)
	{
		names[i][0] = n[i].GetName().FullName();
		names[i][1] = n[i].GetClassification();
	}
	return names;
}

void SetStrings(vector<vector<CString> > strs,Nurses& n)
{
	for (int i=0;i<strs.size();i++)
	{
		n[i].SetClassification(strs[i][1]);
	}
}

vector<vector<double> > GetHours(const Nurses& n)
{
	vector<vector<double> > hours;
	for (int i = 0;
		 i < n.Size();
		 i++)
	{
		vector<double> tempWorker(6);
		tempWorker[0] = n[i].GetDailyHours();
		tempWorker[1] = n[i].GetWeeklyHours();
		tempWorker[2] = n[i].GetPayPeriodHours();
		tempWorker[3] = n[i].GetStartTime();		
		tempWorker[4] = n[i].FixedSchedules().GetNumberOfWeeks();
		tempWorker[5] = n[i].FixedSchedules().GetOffset();
		hours.push_back(tempWorker);
	}
	return hours;
}

void SetHours(const vector<vector<double> >& hours,Nurses& n)
{
	if (hours.size() != n.Size())
		return;
	for (int i=0;i<hours.size();i++)
	{
		n[i].SetDailyHours((int)hours[i][0]);
		n[i].SetWeeklyHours((int)hours[i][1]);
		n[i].SetPayPeriodHours((int)hours[i][2]);
		n[i].SetStartTime((int)hours[i][3]);
		n[i].FixedSchedules().SetNumberOfWeeks((int)hours[i][4]);
		n[i].FixedSchedules().SetOffset((int)hours[i][5]);
	
	}
}

void NameDlg::UpdateDialog()
{
	CRect rc = GetRect(IDC_NAMELIST,this);
	int row=-1;
	int column=-1;
	if (m_pGrid.get())
	{
		SetHours(m_pGrid->GetValues(),m_nurses);
		SetStrings(m_pGrid->GetStrings(),m_nurses);
		this->m_pGrid->GetCurrentCell(row,column);
	}

	if (GetStrings(m_nurses).empty())
		m_pGrid.reset();
	else
	{
		vector<bool> editable(2);
		editable[0] = false;
		editable[1] = true;
		m_pGrid = auto_ptr<CFTSGridCtrl>
		(new CFTSGridCtrl(rc,
		this,1234,GetStrings(m_nurses),
		GetHours(m_nurses),editable));
		m_pGrid->SetCurrentCell(row,column);
	}
	UpdateAll();
	UpdateData(FALSE);
}

void NameDlg::OnBnClickedAdd()
{
	UpdateData(TRUE);
	try
	{
		UpdateDialog();
		m_pGrid.reset();
		CString newName;
		m_newName.GetWindowText(newName);
		m_nurses.Add(Nurse(newName));
		UpdateDialog();
	}
	MY_CATCH()
}

void NameDlg::OnBnClickedRemove()
{
	if (!m_pGrid.get())
		return;
	UpdateDialog();
	int row;
	int column;
	this->m_pGrid->GetCurrentCell(row,column);
	int i = row;
	if (i== -1)
		return;
	m_pGrid.reset();
	m_nurses.Remove(i);
	NameUpdate();
	UpdateDialog();
}

BOOL NameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateDialog();
	return TRUE;  
}

void NameDlg::OnBnClickedClose()
{
	UpdateDialog();
	CDialog::OnOK();
}

void NameDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	UpdateDialog();
}

void NameDlg::OnBnClickedApply()
{
	UpdateDialog();
	
}

void NameDlg::OnBnClickedEditFixed()
{
	UpdateDialog();
	if (!m_pGrid.get())
		return;

	int row;
	int column;
	this->m_pGrid->GetCurrentCell(row,column);
	int i = row;
	if (i== -1)
		return;
	UpdateDialog();
	FixedScheduleDlg dlg(m_nurses[i].GetName().FullName(),m_nurses[i].FixedSchedules(),
		m_nurses[i].GetStartTime(),
		m_nurses[i].GetDailyHours());
	if (IDOK == dlg.DoModal())
		m_nurses[i].FixedSchedules() = dlg.m_fixedSchedule;	
}

void NameDlg::OnBnClickedEditName()
{
	if (!m_pGrid.get())
		return;

	int row;
	int column;
	m_pGrid->GetCurrentCell(row,column);
	if (row == -1)
		return;

	EditNameDlg dlg(m_nurses[row].GetName().FullName());
	if (dlg.DoModal() == IDOK)
	{
		try
		{
			UpdateDialog();
			m_pGrid.reset();
			m_nurses[row].SetName(Name(dlg.m_newName));
			NameUpdate();
			m_nurses.Resort();
			UpdateDialog();
		}
		MY_CATCH()
	}
}
