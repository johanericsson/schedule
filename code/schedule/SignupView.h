#pragma once
#include "assert.h"
#include "period.h"
#include "schedule.h"
#include "nurse.h"
#include "Optional.H"
#include "updater.H"
#include "functions.h"
#include "polyptr.h"



struct UndoStep
{
	UndoStep(const DailyOverrides overrides,CTime day,const CString& desc):
		description(desc),
		day(day),
		overrides(overrides)
		{
		}
	DailyOverrides overrides;
	CTime day;
	CString description;
};

inline CString TimeToString(int time)
{
	assert(time >=0 && time < 24);
	bool isPM = false;
	if (time > 12)
		isPM = true;

	CString hour;
	hour.Format(_T("%d:00 "),isPM ? time-12 : time);
	if (isPM)
		hour+= _T("PM");
	else
		hour+= _T("AM");
	return hour;
}
class SignupView;
struct PickableObject
{
	virtual bool IsInPosition(CPoint point) = 0;
	virtual void OnLButtonDown(SignupView* parent,
		UINT nFlags, CPoint point) = 0;
	virtual PickableObject * Clone() const = 0;
	virtual void Clear(){}
	virtual ~PickableObject(){}
};

class SignupView : public CScrollView,
				   public UpdateSubject
{
	void DrawAvailability(CDC * pDC,CRect rcContainer,int startTime,int stopTime);
	void ClearSelection();
	void DrawVacations(CDC* pDC,CRect rcPos);
	CString VacationString(int num);
	void UpdateTheSubject()
	{
		ClearSelection();
		Invalidate();
	}
	DECLARE_DYNCREATE(SignupView)
	void OnInitialUpdate();
	void OnDraw(CDC * pDC);
	Period& GetPeriod();
	
	DailyOverrides& GetDaily();
public:
	SignupView();
	void DrawSlots(CDC* pDC,
		CPoint& startingPoint,ShiftSetup setup);

	CString GetFilledPosition(int position,
							  int time);
	CString GetDayName() const
	{
		return m_date.Format(_T("%A")); // return weekday name
	}
	CString GetDate() const
	{
		return m_date.Format(_T("%b %d, %Y"));
	}
	CTime m_date;
	CString AvailabilitySlotString(int num,int startTime,int stopTime,
		Optional<Slot>&);



	vector<UndoStep> m_undoSteps;
	void AddUndoStep(const CString& desc);
	void RemoveUndoStep();
	afx_msg void OnUndo();

	bool m_bCollapsed;

	void AddPickableObject(const PickableObject& obj)
	{
		m_entries.push_back(obj);
	}
	vector<poly_ptr<PickableObject> > m_entries;

	Optional<poly_ptr<PickableObject> > m_selection;

	void DrawHeading(CDC* pDC,const CString& heading,CRect rcPos,int startTime,
							 int stopTime);


	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPreviousDay();
	afx_msg void OnResetDay();
	afx_msg void OnNextDay();
	afx_msg void OnDestroy();
	afx_msg void OnPrintOne();
	afx_msg void OnPrintall();
	afx_msg void OnAssignments();
	afx_msg void OnNextWeek();
	afx_msg void OnPreviousWeek();
	afx_msg void OnOverrideSchedule();
	afx_msg void OnViewDailyoverview();
	afx_msg void OnUpdateUndo(CCmdUI * target);
	afx_msg void OnCollapse();
	afx_msg void OnUpdateCollapse(CCmdUI *pCmdUI);
};