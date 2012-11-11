#pragma once
#include "updater.h"
class Period;
class WeekView : public CScrollView,
				 public UpdateSubject
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(WeekView)
public:

	WeekView():
	  m_weekNumber(0)
	  {
	  }

	void Draw(const Period& period,
							int weekNumber,CDC* pDC);
	int m_weekNumber;
	void UpdateTheSubject()
	{
		Invalidate();
	}

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnInitialUpdate();
	
	void OnDraw(CDC * pDC);

	afx_msg void OnNext();
	afx_msg void OnPrevious();
};