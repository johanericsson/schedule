#pragma once
#include "afxwin.h"


// AddVacationDlg dialog

class AddVacationDlg : public CDialog
{
public:
	AddVacationDlg(const Nurse& nurse,
		vector<VacationSchedule>& vacationSchedules);

	const Nurse& m_nurse;
	vector<VacationSchedule>& m_vacationSchedules;
// Dialog Data
	enum { IDD = IDD_ADD_VACATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	VacationSchedule m_sched;
	CComboBox m_typesCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
