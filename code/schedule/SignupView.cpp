#include "stdafx.h"
#include "signupview.h"
#include "functions.h"
#include "optional.h"
#include "name.h"
#include "signupview.h"
#include "DailyAssignmentsdlg.h"
#include "ScheduleOverrideDlg.h"
#include "dailyoverview.h"
#include "ChooseNameDlg.h"
#include "addvacationdlg.h"
#include "addentrydlg.h"
#include "MyException.h"
#include ".\signupview.h"

void SignupView::AddUndoStep(const CString& desc)
{
	m_undoSteps.push_back(UndoStep(GetDaily(),m_date,desc));
	if (m_undoSteps.size() > 100)
	{
		m_undoSteps.erase(m_undoSteps.begin());
	}
}

void SignupView::RemoveUndoStep()
{
	m_undoSteps.erase(m_undoSteps.end() - 1);
}

void SignupView::OnUpdateUndo(CCmdUI * target)
{
	target->Enable(!m_undoSteps.empty());
}
void SignupView::OnUndo()
{
	if (m_undoSteps.empty())
		return;
	m_date = m_undoSteps.back().day;
	GetDaily() = m_undoSteps.back().overrides;
	m_undoSteps.erase(m_undoSteps.end() - 1);
	UpdateAll();
}


CRect LPtoDP(CDC* pDC,const CRect& rcPos)
{
	CRect rcNew = rcPos;
	pDC->LPtoDP(&rcNew);
	rcNew.NormalizeRect();
	return rcNew;
}

Period& SignupView::GetPeriod()
{
	return static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
}

DailyOverrides& SignupView::GetDaily()
{
	Period& period = GetPeriod();
	CTimeSpan span = m_date - period.m_dateRange.GetStartDate();
	int days = static_cast<int>(span.GetDays());
	return period.m_data.m_assignments[days];
}


IMPLEMENT_DYNCREATE(SignupView,CScrollView)
static const int width = 11000;
static const int height = 8500;



struct PickableAvailableEntry : public PickableObject
{
	CRect rcPos;
	Slot slot;
	PickableAvailableEntry(CRect rcPos,const Slot& slot):
		rcPos(rcPos),
		slot(slot)
	{
	}
	virtual bool IsInPosition(CPoint point)
	{
		return rcPos.PtInRect(point);
	}
	virtual PickableObject * Clone() const
	{
		return new PickableAvailableEntry(*this);
	}
	virtual void OnLButtonDown(SignupView * view,
		UINT nFlags, CPoint point)
	{
		DailyOverrides& daily  = view->GetDaily();
		for (int i=0;i<daily.m_overrides.size();i++)
		{
			if (daily.m_overrides[i].m_nurse == slot.name)
			{
				ASSERT(0); // should not already exist here.
				daily.m_overrides.erase(daily.m_overrides.begin() + i);
			}
		}
		view->AddUndoStep(_T("Removed available"));
		daily.m_overrides.push_back(slot.name);
		UpdateAll();
	}
};

CString SignupView::AvailabilitySlotString(int num,int startTime,int stopTime,Optional<Slot>& outName)
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	vector<Slot> available = GetAvailableNurses(period, m_date);
	
	CString avail;
	avail.Format(_T("%d. "),num);
	
	int count = 0;
	CString name(_T("_______________"));
	for (int i=0;i<available.size();i++)
	{
		if (available[i].startTime >= startTime &&
			available[i].startTime < stopTime)
		{
			count++;
			if (count == num)
			{
				name = available[i].name.FullName();
				outName = available[i];
			}
		}
	}

	avail+= name;
	return avail;
}

void SignupView::
DrawAvailability(CDC * pDC,CRect rcContainer,int startTime,int stopTime)
{
	CFont arial;
	VERIFY(arial.CreatePointFont(1200,_T("Arial")));
	CFont * oldFont = pDC->SelectObject(&arial);

	CString heading = _T("Availability/Extra Shifts");
	CRect rcSize = rcContainer;
	pDC->DrawText(heading,&rcSize,DT_CALCRECT);
	int headingWidth = rcSize.Width();
	int left = rcContainer.CenterPoint().x - headingWidth/2;
	int top = rcContainer.top - 10;
	pDC->DrawText(heading,CRect(left,top,left,top),DT_NOCLIP);

	int wholeTop = rcSize.bottom - 200;
	int heightDistance = (wholeTop - rcContainer.bottom) / 3;
	const int Rows = 1;
	for (int i=0;i<Rows;i++)
	{
		int top = wholeTop - heightDistance*i;
		CRect rcSlotLocation = CRect(rcContainer.left+20,top
			,rcContainer.left+20,top);
		Optional<Slot> outName;
		CString slotString = AvailabilitySlotString(i+1,startTime,stopTime,outName);
		
		pDC->DrawText(slotString,rcSlotLocation,DT_NOCLIP);
		pDC->DrawText(slotString,&rcSlotLocation,DT_CALCRECT | DT_NOCLIP);
		if (outName)
			AddPickableObject(PickableAvailableEntry(LPtoDP(pDC,rcSlotLocation),*outName));

		int left = rcContainer.CenterPoint().x;
		outName = Empty();
		slotString = AvailabilitySlotString(i+Rows+1,startTime,stopTime,outName);
		rcSlotLocation = CRect(left,top,left,top);
		pDC->DrawText(slotString,rcSlotLocation,DT_NOCLIP);
		pDC->DrawText(slotString,&rcSlotLocation,DT_CALCRECT | DT_NOCLIP);
		if (outName)
			AddPickableObject(PickableAvailableEntry(LPtoDP(pDC,rcSlotLocation),*outName));
	}
	CFont * tmpFont = pDC->SelectObject(oldFont);
	ASSERT(tmpFont = &arial);
}


CString SignupView::
VacationString(int num)
{
	CString vacationText;
	vector<VacationSchedule>& vacations = GetDaily().m_vacationSchedules;
	vacationText.Format(_T("%d. "),num+1);
	if (num >= vacations.size())
	{
		vacationText += _T("_______________");
		return vacationText;
	}
	
	VacationSchedule& instance = vacations[num];
	vacationText += instance.m_nurse.FullName();
	vacationText += _T(":");
	vacationText += instance.m_code;
	return vacationText;
}


struct PickableVacationEntry : public PickableObject
{
	int entry;
	CRect rcPos;
	PickableVacationEntry(CRect rcPos,int entry):
rcPos(rcPos),entry(entry)
	{
	};

	virtual bool IsInPosition(CPoint point)
	{
		return rcPos.PtInRect(point);
	}
	virtual PickableObject* Clone()const
	{
		return new PickableVacationEntry(*this);
	}

	virtual void OnLButtonDown(SignupView * view,
		UINT nFlags, CPoint point)
	{
		// Remove the vacation day
		vector<VacationSchedule>& schedules = view->GetDaily().m_vacationSchedules;
		if (entry >= schedules.size())
		{
			return;
		}
		view->AddUndoStep(_T("Removed vacation entry"));
		schedules.erase(schedules.begin() + entry);
		UpdateAll();
	}


};

struct PickableVacationHeading : public PickableObject
{
	CRect rcPos;
	PickableVacationHeading(CRect rcPos):rcPos(rcPos)
	{
	}
	
	virtual bool IsInPosition(CPoint point)
	{
		return rcPos.PtInRect(point);
	}
	virtual void OnLButtonDown(SignupView * view,
		UINT nFlags, CPoint point)
	{
		Optional<Nurse> nurse = 
			ChooseNurse(GetPeriod().m_data.m_nurses.m_nurses,view->m_date);
		if (nurse)
		{
			AddVacationDlg dlg(*nurse,view->GetDaily().m_vacationSchedules);
			if (IDOK==dlg.DoModal())
			{
				view->AddUndoStep(_T("Adding vacation entry"));
				view->GetDaily().m_vacationSchedules.push_back(dlg.m_sched);
				UpdateAll();
			}
			
		}
	}
	virtual PickableObject * Clone() const
	{
		return new PickableVacationHeading(*this);
	}
	virtual void Clear(){}
};

void SignupView::
DrawVacations(CDC* pDC,CRect rcPos)
{
	CFont arial;
	VERIFY(arial.CreatePointFont(1200,_T("Arial")));
	CFont * oldFont = pDC->SelectObject(&arial);

	CString heading = _T("Vacations/etc.");
	CRect rcSize = rcPos;
	pDC->DrawText(heading,&rcSize,DT_CALCRECT);
	int headingHeight = rcSize.Height();
	CRect rcHeading = rcPos;
	int headingWidth = rcSize.Width();
	rcHeading.OffsetRect(rcPos.CenterPoint().x - headingWidth/2,
		headingHeight/2);
	pDC->DrawText(heading,rcHeading,DT_NOCLIP);
	rcSize = rcHeading;
	pDC->DrawText(heading,&rcSize,DT_CALCRECT);
	AddPickableObject(PickableVacationHeading(LPtoDP(pDC,rcSize)));
	for (int x = 0; x< 5;x++)
	for (int y = 0;y<2;y++)
	{
		CString slot = VacationString(x+5*y);
		int left = static_cast<int>((rcPos.Width() / 5.0) * x + 20);
		int top = static_cast<int>(rcPos.top + headingHeight*1.5+ headingHeight*y*1.5);
		CRect rcEntry = CRect(left,top,left,top);
		pDC->DrawText(slot,rcEntry,DT_NOCLIP);
		pDC->DrawText(slot,&rcEntry,DT_CALCRECT | DT_NOCLIP);
		AddPickableObject(PickableVacationEntry(LPtoDP(pDC,rcEntry),x));
	}
	CFont * tmpFont = pDC->SelectObject(oldFont);
	ASSERT(tmpFont == &arial);
}

struct Entry : PickableObject
{
	PickableObject * Clone() const {return new Entry(*this);}
	virtual bool IsInPosition(CPoint point)
	{
		return position.PtInRect(point);
	}
	virtual void OnLButtonDown(SignupView* view,UINT nFlags, CPoint point)
	{
		if (!workingEntry)
			return;
		DailyOverrides& daily = view->GetDaily();
		if (!workingEntry->fixed)
		{
			// find the nurse and remove him from the assignments
			for (int i=0;i<daily.Size();i++)
			{
				if (daily[i].GetName() == workingEntry->name)
				{
					view->AddUndoStep(_T("Removed assignment"));
					daily.Remove(i);
					UpdateAll();
					return;
				}
			}
			ASSERT(0); // should have found it
			return;
		}
		for (int i=0;i<daily.m_overrides.size();i++)
		{
			if (daily.m_overrides[i].m_nurse == workingEntry->name)
			{
				ASSERT(0); // should not already exist here.
				daily.m_overrides.erase(daily.m_overrides.begin() + i);
			}
		}
		view->AddUndoStep(_T("Removed fixed schedule"));
		daily.m_overrides.push_back(FixedScheduleOverride(workingEntry->name));
		UpdateAll();
	}
	Optional<Slot> workingEntry;
	CRect position;
	int number;
};

void SignupView::
DrawSlots(CDC* pDC,CPoint& startingPoint,ShiftSetup setup)
{
	Period& period = static_cast<CScheduleApp*>
		(AfxGetApp())->m_periods.GetCurrentPeriod();
	CString time = _T("Time: ");
	time += TimeToString(setup.time);
	pDC->DrawText(time,CRect(startingPoint,startingPoint),DT_NOCLIP);
	startingPoint.y -= 500;
	CFont arial;
	VERIFY(arial.CreatePointFont(1600,_T("Arial")));
	CFont * oldFont = pDC->SelectObject(&arial);

	pDC->DrawText(_T("Need:"),CRect(startingPoint,startingPoint),DT_NOCLIP);

	startingPoint.x += 800;
	for (int i=0;i<(setup.emptySlots.size() + setup.NumberOfSpotsNeeded());i++)
	{
		CString number;
		number.Format(_T("%d. "),i+1);
		Entry entry;
		if (i<setup.NumberOfSpotsNeeded())
			entry.workingEntry = setup.workers[i];
		
		if (entry.workingEntry)
		{
			number += entry.workingEntry->name.FullName();
			if (entry.workingEntry->extraWorker)
			{
				pDC->SetTextColor(RGB(255,0,0));
			}
			else
			if (entry.workingEntry->fixed)
			{
				pDC->SetTextColor(RGB(0,0,0));
			}
			else
				pDC->SetTextColor(RGB(100,100,0));
		}
		else
		{
			number += _T("_____________");
		}
		CRect rcPosition(startingPoint,startingPoint);
		pDC->DrawText(number,rcPosition,DT_NOCLIP);
		pDC->DrawText(number,&rcPosition,DT_NOCLIP | DT_CALCRECT);
		
		// Output number of hours:
		int hours;
		if (entry.workingEntry)
		{
			hours = entry.workingEntry->hours;
		}
		else
			hours = setup.emptySlots[i - setup.NumberOfSpotsNeeded()].hours;
		CString hoursStr;
		hoursStr.Format(_T("%dH"),hours);
		CRect rcNum(rcPosition.right+20,rcPosition.top,
			rcPosition.right+20,
			rcPosition.top);
		pDC->DrawText(hoursStr,rcNum,DT_NOCLIP);
		pDC->DrawText(hoursStr,rcNum,DT_NOCLIP | DT_CALCRECT);
		pDC->LPtoDP(rcNum);
		//Hours hoursRemember;
		//hoursRemember.entry = entry;
		//hoursRemember.position = rcNum;
		//m_entries.push_back(hoursRemember);
		
		pDC->LPtoDP(&rcPosition);
		entry.position = rcPosition;
		entry.number = i;
		m_entries.push_back(entry);
		pDC->SetTextColor(RGB(0,0,0));


		startingPoint.y -= 300;
	}
	startingPoint.x -= 800;
	
	CFont *tmpFont = pDC->SelectObject(oldFont);
	ASSERT(tmpFont = &arial);
}


void SignupView::ClearSelection()
{
	if (m_selection)
	{
		(*m_selection)->Clear();
	}
	m_selection = Empty();
}

struct PickableHeading : public PickableObject
{
	CRect rcPos;
	int stopTime;
	int startTime;
	PickableHeading(CRect rcPos,int startTime,int stopTime):
		startTime(startTime),
		rcPos(rcPos),
		stopTime(stopTime)
		{
		}
	bool IsInPosition(CPoint point)
	{
		return rcPos.PtInRect(point);
	}
	PickableObject* Clone() const
	{
		return new PickableHeading(*this);
	}
	virtual void OnLButtonDown(SignupView* view,
		UINT nFlags, CPoint point)
	{
		vector<Nurse>& oldNurses = GetPeriod().m_data.m_nurses.m_nurses;
		vector<Nurse> sortedNurses;
		int sortedPos = 0;
		for (int i=0;i<oldNurses.size();i++)
		{
			Nurse& nurse = oldNurses[i];
			if (startTime <= nurse.GetStartTime() &&
				nurse.GetStartTime() < stopTime)
			{
				sortedNurses.insert(sortedNurses.begin() + 
					sortedPos++,nurse);
			}
			else
				sortedNurses.push_back(nurse);
		}
		Optional<Nurse> nurse = ChooseNurse(sortedNurses,view->m_date);
		if (!nurse)
			return;
		AddEntryDlg dlg(*nurse);
		if (IDOK != dlg.DoModal())
			return;

		view->AddUndoStep(_T("Adding schedule Entry for ") + nurse->GetName().FullName());
		try
		{
			view->GetDaily().Add(Assignment(nurse->GetName(),dlg.m_startTime,dlg.m_numberOfHours));
		}
		catch(OurException& ex)
		{
			AfxMessageBox(ex.m_str);
			view->RemoveUndoStep();
		}
		UpdateAll();
	};
};

void SignupView::DrawHeading(CDC* pDC,const CString& heading,CRect rcPos,int startTime,
							 int stopTime)
{

	pDC->DrawText(heading,rcPos,DT_NOCLIP);
	pDC->DrawText(heading,&rcPos,DT_CALCRECT | DT_NOCLIP);
	AddPickableObject(PickableHeading(LPtoDP(pDC,rcPos),startTime,stopTime));
}

void SignupView::OnDraw(CDC * pDC)
{
	CPen pen(PS_SOLID,5,RGB(0,0,0));
	CPen * oldPen = pDC->SelectObject(&pen);

	m_entries.clear();
	CFont arial;
	VERIFY(arial.CreatePointFont(2400,_T("Arial")));
	CFont * oldFont = pDC->SelectObject(&arial);
	
	const int YDivider1 = -750;
	//const int YDivider2 = -4750;
	const int YDivider2 = -6400;
	const int XDivider1 = width/3;
	const int XDivider2 = width*2/3;

	const int YDivider3 = -height+1250;
	// There are three sections
	
	pDC->MoveTo( 0, YDivider1);
    pDC->LineTo( width,YDivider1);

	pDC->MoveTo(0,YDivider2);
	pDC->LineTo(width,YDivider2);

	pDC->MoveTo(XDivider1,YDivider1);
	pDC->LineTo(XDivider1,YDivider3);

	pDC->MoveTo(XDivider2,YDivider1);
	pDC->LineTo(XDivider2,YDivider3);

	pDC->MoveTo(0,YDivider3);
	pDC->LineTo(width,YDivider3);

	const int headerYPosition = -250;
	pDC->DrawText(GetDayName(),CRect(1000,headerYPosition,
		1000,headerYPosition),DT_NOCLIP);
	CString date = _T("Date: ");
	date += GetDate();
	pDC->DrawText(date,CRect(5000,headerYPosition,
		7000,headerYPosition),DT_NOCLIP);

	DrawHeading(pDC,_T("Days"),CRect(100,YDivider1-100,100,YDivider1-300),0,11);
	DrawHeading(pDC,_T("PM's"),CRect(XDivider1+100,YDivider1-100,XDivider1+100,YDivider1-100),11,19);
	DrawHeading(pDC,_T("NOC's"),CRect(XDivider2+100,YDivider1-100,XDivider2+100,YDivider1-100),19,24);

	CPoint point(100,YDivider1-500);

	Period& period = GetPeriod();
	vector<ShiftSetup> days = GetShifts(period,m_date,0,11);
	vector<ShiftSetup> pms= GetShifts(period,m_date,11,19);
	vector<ShiftSetup> nocs = GetShifts(period,m_date,19,80);
	for (int i=0;i<days.size();i++)
	{
		DrawSlots(pDC,point,days[i]);
	}
	point = CPoint(XDivider1 + 100,YDivider1-500);
	for (int i=0;i<pms.size();i++)
	{
		DrawSlots(pDC,point,pms[i]);
	}
	point = CPoint(XDivider2 + 100,YDivider1-500);
	for (int i=0;i<nocs.size();i++)
	{
		DrawSlots(pDC,point,nocs[i]);
	}

	DrawAvailability(pDC,CRect(0,YDivider2,XDivider1,YDivider3),0,11);
	DrawAvailability(pDC,CRect(XDivider1,YDivider2,XDivider2,YDivider3),11,19);
	DrawAvailability(pDC,CRect(XDivider2,YDivider2,width,YDivider3),19,24);

	DrawVacations(pDC,CRect(0,YDivider3,width,-height));

	CPen * tempPen = pDC->SelectObject(oldPen);
	ASSERT(tempPen == &pen);
	CFont *tmpFont = pDC->SelectObject(oldFont);
	ASSERT(tmpFont = &arial);
}

void SignupView::OnInitialUpdate()
{
	SetScrollSizes(MM_HIENGLISH,CSize(width,height));
	GetParent()->SetWindowText(_T("Signup View"));
	CScrollView::OnInitialUpdate();
	
}
BEGIN_MESSAGE_MAP(SignupView, CScrollView)
ON_WM_CREATE()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONUP()
ON_COMMAND(ID_PREVIOUS, OnPreviousDay)
ON_COMMAND(ID_NEXT, OnNextDay)
ON_COMMAND(ID_FAST_FORWARD, OnNextWeek)
ON_COMMAND(ID_FAST_REVERSE, OnPreviousWeek)
ON_WM_DESTROY()
ON_COMMAND(ID_PRINT_ONE, OnPrintOne)
ON_COMMAND(ID_PRINTALL, OnPrintall)
ON_COMMAND(ID_ASSIGNMENTS, OnAssignments)
ON_COMMAND(ID_OVERRIDE_SCHEDULE, OnOverrideSchedule)
ON_COMMAND(ID_RESET_DAY,OnResetDay)
ON_COMMAND(ID_VIEW_DAILYOVERVIEW, OnViewDailyoverview)
ON_COMMAND(ID_UNDO,OnUndo)
ON_UPDATE_COMMAND_UI(ID_UNDO,OnUpdateUndo)
ON_COMMAND(ID_COLLAPSE, OnCollapse)
ON_UPDATE_COMMAND_UI(ID_COLLAPSE, OnUpdateCollapse)
END_MESSAGE_MAP()

int SignupView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnInitialUpdate();

	return 0;
}

void SignupView::OnLButtonUp(UINT nFlags, CPoint point)
{
	for (int i=0;i<m_entries.size();i++)
	{
		if (m_entries[i]->IsInPosition(point))
		{
			ClearSelection();
			m_selection = m_entries[i];
			(*m_selection)->OnLButtonDown(this,nFlags,point);
			break;
		}
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void SignupView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnRButtonUp(nFlags, point);
}

SignupView::SignupView():
m_bCollapsed(false)
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	m_date = period.m_dateRange.GetStartDate();
}

void SignupView::OnPreviousDay()
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date - CTimeSpan(1,0,0,0)) >= period.m_dateRange.GetStartDate())
	m_date -= CTimeSpan(1,0,0,0);
	Invalidate();
}

void SignupView::OnNextDay()
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date + CTimeSpan(1,0,0,0)) < period.m_dateRange.GetEndDate())
		m_date += CTimeSpan(1,0,0,0);
	Invalidate();
}

void SignupView::OnNextWeek()
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date + CTimeSpan(7,0,0,0)) < period.m_dateRange.GetEndDate())
		m_date += CTimeSpan(7,0,0,0);
	else
		m_date = period.m_dateRange.GetEndDate() - CTimeSpan(1,0,0,0);
	
	Invalidate();
}

void SignupView::OnPreviousWeek()
{
	Period& period = static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
	if ((m_date - CTimeSpan(7,0,0,0)) > period.m_dateRange.GetStartDate())
		m_date -= CTimeSpan(7,0,0,0);
	else
		m_date = period.m_dateRange.GetStartDate();
	Invalidate();
}

void SignupView::OnDestroy()
{
	ClearSelection();
	__super::OnDestroy();
}

void SignupView::OnPrintOne()
{
	CPrintDialog dlg(FALSE);
	if (IDOK == dlg.DoModal())
	{
		HDC dc = dlg.GetPrinterDC();
		
		CDC DC;
		DC.Attach(dc);

		DEVMODE *myMode = dlg.GetDevMode();//fills myMode with printer defaults 
		myMode->dmOrientation = DMORIENT_LANDSCAPE;//change default to landscape
		myMode->dmPrintQuality = DMRES_DRAFT;
		myMode->dmColor = DMCOLOR_MONOCHROME;
		DC.ResetDC(myMode);


		DC.m_bPrinting = TRUE;
		OnPrepareDC(&DC);
		
		DC.StartDoc(_T("myDoc"));
		DC.StartPage();
		
		OnDraw(&DC);
		DC.EndPage();
		DC.EndDoc();
	}
}

void SignupView::OnPrintall()
{
	CPrintDialog dlg(FALSE);
	if (IDOK == dlg.DoModal())
	{
		HDC dc = dlg.GetPrinterDC();
		
		

		CDC DC;
		DC.Attach(dc);
		
		DEVMODE *myMode = dlg.GetDevMode();//fills myMode with printer defaults 
		myMode->dmOrientation = DMORIENT_LANDSCAPE;//change default to landscape
		myMode->dmPrintQuality = DMRES_DRAFT;
		myMode->dmColor = DMCOLOR_MONOCHROME;
		DC.ResetDC(myMode);

		DC.m_bPrinting = TRUE;
		OnPrepareDC(&DC);
		
		DC.StartDoc(_T("myDoc"));
		Period& period = 
				static_cast<CScheduleApp*>(AfxGetApp())->m_periods.GetCurrentPeriod();
		CTimeSpan span = period.m_dateRange.GetEndDate() - period.m_dateRange.GetStartDate();
		int days = (int)span.GetDays();
		CTime lastDay = period.m_dateRange.GetEndDate();
		m_date = period.m_dateRange.GetStartDate();
		for (m_date=period.m_dateRange.GetStartDate()
			;m_date<lastDay;
			m_date+= CTimeSpan(1,0,0,0))
		{
			DC.StartPage();
			OnDraw(&DC);
			DC.EndPage();
		}
		DC.EndDoc();
	}
}

void SignupView::OnAssignments()
{	
	CTimeSpan span = m_date - GetPeriod().m_dateRange.GetStartDate();
	DailyAssignmentsDlg dlg(GetPeriod(),(int)span.GetDays());
	dlg.DoModal();
	UpdateAll();
}

void SignupView::OnOverrideSchedule()
{
	CTimeSpan span = m_date - GetPeriod().m_dateRange.GetStartDate();
	ScheduleOverrideDlg dlg((int)span.GetDays());
	dlg.DoModal();
	UpdateAll();
}

void SignupView::OnViewDailyoverview()
{
	CTimeSpan span = m_date - GetPeriod().m_dateRange.GetStartDate();
	DailyOverview::m_dayInitializer = (int)span.GetDays();
	CScheduleApp* app = static_cast<CScheduleApp*>(AfxGetApp());
	CFrameWnd * newWnd = CreateNewView(app->m_pDailyOverview);
	DailyOverview::m_dayInitializer = 0;
}

void SignupView::OnResetDay()
{
	long ret = AfxMessageBox(_T("Are you sure you would like to remove all modificaions for\n")
				  _T("this day?"),MB_OKCANCEL);
	if (ret== IDOK)
	{
		AddUndoStep(_T("Reset"));
		GetDaily() = DailyOverrides();
		UpdateAll();
	}
}
void SignupView::OnCollapse()
{
	m_bCollapsed = !m_bCollapsed;
	Invalidate();
}

void SignupView::OnUpdateCollapse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bCollapsed);
}
