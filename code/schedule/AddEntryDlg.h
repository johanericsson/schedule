#pragma once


// AddEntryDlg dialog

class AddEntryDlg : public CDialog
{
	DECLARE_DYNAMIC(AddEntryDlg)

public:
	AddEntryDlg(const Nurse& nurse);   // standard constructor

// Dialog Data
	enum { IDD = IDD_ADD_ENTRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_startTime;
	int m_numberOfHours;
	CString m_name;
	afx_msg void OnBnClickedOk();
};
