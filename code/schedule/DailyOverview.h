#pragma once
#include "updater.h"

class DailyOverview : public CScrollView,
					  public UpdateSubject

{
	int m_day;
	DECLARE_DYNCREATE(DailyOverview)
	DECLARE_MESSAGE_MAP()
public:
	void OnDraw(CDC *pDC);
	int OnCreate(LPCREATESTRUCT lpcs);
	DailyOverview():
	  m_day(m_dayInitializer)
	{
	}
	void UpdateTheSubject()
	{
		Invalidate();
	}

	void OnNextWeek();
	void OnPreviousWeek();
	static int m_dayInitializer;
	afx_msg void OnNextDay();
	afx_msg void OnPreviousDay();
};