#pragma once


#include "updater.h"
class NeedsView : public CScrollView,
				 public UpdateSubject
{

	DECLARE_DYNCREATE(NeedsView)
	DECLARE_MESSAGE_MAP()
	void GetString(int maxNumberOfLines);
	void UpdateTheSubject()
	{
		m_output.clear();
		Invalidate();
	}
	
	NeedsView():m_currentPage(0)
	{
	}
	int m_currentPage;

	vector<CString> m_output;
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDraw(CDC* pDC);
public:
	afx_msg void OnNextDay();
	afx_msg void OnUpdateNextDay(CCmdUI *pCmdUI);
	afx_msg void OnPreviousDay();
	afx_msg void OnUpdatePreviousDay(CCmdUI *pCmdUI);
	afx_msg void OnPrintall();
};