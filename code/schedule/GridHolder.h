#pragma once

#include "ftsGridctrl.h"
#include <memory>
// GridHolder dialog

class GridHolder : public CDialog
{
	auto_ptr<CFTSGridCtrl> m_pGridCtrl;
	DECLARE_DYNAMIC(GridHolder)

public:
	GridHolder(CWnd* pParent = NULL);   // standard constructor
	virtual ~GridHolder();

// Dialog Data
	enum { IDD = IDD_GRID_HOLDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
