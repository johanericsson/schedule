// AddVacationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Schedule.h"
#include "AddVacationDlg.h"
#include ".\addvacationdlg.h"

AddVacationDlg::AddVacationDlg(const Nurse& nurse,
							   vector<VacationSchedule>& 
							   vacationSchedules):
							   m_nurse(nurse),
	 CDialog(AddVacationDlg::IDD, NULL),
	m_vacationSchedules(vacationSchedules)
{
}

void AddVacationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYPE, m_typesCtrl);
}


BEGIN_MESSAGE_MAP(AddVacationDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// AddVacationDlg message handlers

BOOL AddVacationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	Period& period = ((CScheduleApp*)AfxGetApp())->m_periods.GetCurrentPeriod();
	
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_nurse.GetName().FullName());
	VacationTypes& types = period.m_data.m_vacationTypes;
	for (int i=0;i<types.Size();i++)
	{
		m_typesCtrl.AddString(ToString(types[i]));
	}
	if (types.Size() > 0)
		m_typesCtrl.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AddVacationDlg::OnBnClickedOk()
{
	int typeSel = m_typesCtrl.GetCurSel();
	if (typeSel == LB_ERR)
		return;

	Period& period = ((CScheduleApp*)AfxGetApp())->m_periods.GetCurrentPeriod();
	Nurses& nurses = period.m_data.m_nurses;
	VacationTypes& types = period.m_data.m_vacationTypes;

	
	m_sched.m_code = types[typeSel].m_code;
	m_sched.m_nurse = m_nurse.GetName().FullName();
	OnOK();
}
