#pragma once

#include "Updater.h"




class ScheduleViewEx : public CScrollView,
						 public UpdateSubject

{
public:
	BOOL OnCreate(LPCREATESTRUCT l);
	DECLARE_DYNCREATE(ScheduleViewEx)
	ScheduleViewEx():
		m_offset(0)
	  {
	  }
	  void OnPrevious();
	  void OnNext();
	  
	CSize m_oldSize;
	  int GetXOffset() const;
	  
	  int GetYOffset() const;
	  
	int m_offset;
	DECLARE_MESSAGE_MAP()
	int TotalPages() const;
	void OnInitialUpdate();
	void OnDraw(CDC *pDC);

	virtual void UpdateTheSubject();

	void OnPrintAll();


};