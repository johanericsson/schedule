#pragma once
#include "afxcmn.h"

#include "optional.h"
// ChooseNameDlg dialog

Optional<Nurse> ChooseNurse(const vector<Nurse>& nurses,CTime date);

class ChooseNameDlg : public CDialog
{
public:
	int m_selectedIndex;
	ChooseNameDlg(const vector<Nurse>& nurses,CTime m_date);

	CTime m_date;
	const vector<Nurse>& m_nurses;
// Dialog Data
	enum { IDD = IDD_CHOOSE_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_nameListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
