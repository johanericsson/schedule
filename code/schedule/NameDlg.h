#pragma once
#include "afxwin.h"
#include "name.h"
#include <memory>
// NameDlg form view
#include "FTSGridCtrl.h"
#include "resource.h"
class Nurses;
class NameDlg : public CDialog
{
	void UpdateDialog();
public:
	NameDlg(Nurses&);           // protected constructor used by dynamic creation
public:
	enum { IDD = IDD_NAMES };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	Nurses& m_nurses;
	CEdit m_newName;
	auto_ptr<CFTSGridCtrl> m_pGrid;
	
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedEditFixed();
	afx_msg void OnBnClickedEditName();
};


