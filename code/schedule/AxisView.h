#pragma once
#include "ScheduleView.h"

class AxisView : public CWnd
{
	DECLARE_MESSAGE_MAP()
public:
	AxisView();
	void OnPaint();
	void OnNcDestroy();
	int m_height;
	int m_width;
	void OnSize(UINT nType,int cx,int cy);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	ScheduleView * m_scheduleView;
	bool m_bFirstSize;
};