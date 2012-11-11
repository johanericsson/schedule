#pragma once
#include "afxwin.h"
#include "VacationTypes.h"


// VacationTypeDlg dialog

class VacationTypeDlg : public CDialog
{
public:
	VacationTypeDlg(VacationTypes& types);   // standard constructor
	VacationTypes m_types;
	VacationTypes& m_oldTypes;
// Dialog Data
	enum { IDD = IDD_VACATION_TYPES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void UpdateListBox();
	CString m_code;
	CString m_description;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedOk();
	CListBox m_typesCtrl;
	virtual BOOL OnInitDialog();
};
