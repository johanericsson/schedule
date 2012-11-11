#pragma once
#include "afxwin.h"
#include "resource.h"
#include "ShiftLegend.h"
// LegendEditorDlg dialog

class LegendEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(LegendEditorDlg)
	ShiftLegend& m_oldLegend;
	ShiftLegend m_legend;
public:
	LegendEditorDlg(ShiftLegend& legend);   // standard constructor
	void UpdateList();
// Dialog Data

	enum { IDD = IDD_LEGEND_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedChange();
	afx_msg void OnBnClickedRemove();
	CString m_abbreviationNameCtrl;
	int m_startTimeCtrl;
	int m_numberOfHoursCtrl;
	CListBox m_listCtrl;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeAbbreviationList();
};
