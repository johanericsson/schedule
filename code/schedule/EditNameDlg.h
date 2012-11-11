#pragma once


// EditNameDlg dialog

class EditNameDlg : public CDialog
{
public:
	EditNameDlg(const CString& oldName);

// Dialog Data
	enum { IDD = IDD_NAME_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_oldName;
	CString m_newName;
};
