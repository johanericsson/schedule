#include "stdafx.h"
#include "needsview.h"
#include "Schedule.h"
#include "functions.h"
#include "signupview.h"
#include ".\needsview.h"

IMPLEMENT_DYNCREATE(NeedsView,CScrollView)

BEGIN_MESSAGE_MAP(NeedsView,CScrollView)
	ON_WM_CREATE()
	ON_COMMAND(ID_NEXT, OnNextDay)
	ON_UPDATE_COMMAND_UI(ID_NEXT, OnUpdateNextDay)
	ON_COMMAND(ID_PREVIOUS, OnPreviousDay)
	ON_UPDATE_COMMAND_UI(ID_PREVIOUS, OnUpdatePreviousDay)
	ON_COMMAND(ID_PRINTALL, OnPrintall)
END_MESSAGE_MAP()

static const int width = 8500;
static const int height = 11000;
int NeedsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct)==-1)
		return -1;
	SetScrollSizes(MM_HIENGLISH,CSize(width,height));
	CString windowTitle;
	windowTitle.Format(_T("Needs View: Page %d"),m_currentPage+1);
	SetWindowText(windowTitle);
	return 0;
}

CString GetNextChunk(Period& period,CTime& current,int maxNumberOfLines)
{
	int currentLineNumber = 0;
	CString chunk;
	CString currentChunk;
	for(;(current!=period.m_dateRange.GetEndDate()) && (currentLineNumber < maxNumberOfLines);
		 current += CTimeSpan(1,0,0,0))
	{
		vector<ShiftSetup> shifts = GetShifts(GetPeriod(),current);
		bool needed = false;
		for (int i=0;i<shifts.size();i++)
		{
			ShiftSetup& shift = shifts[i];
			if (shift.time > 23)
				continue;
			int column = 0;
			const int NUMBER_OF_COLUMNS =2;
			for (int j=0;j<shift.emptySlots.size();j++)
			{
				if (!needed)
				{
					chunk += currentChunk;
					currentChunk = CString();
					currentChunk += current.Format(_T("%A, %B %d, %Y:\n"));
					currentLineNumber++;
					needed = true;
				}
				EmptySlot& slot = shift.emptySlots[j];
				CString currentStr;
				if (j==0)
					currentChunk += TimeToString(shift.time) + _T(":");

				CString line;
				line.Format(_T("%d.____________%dH"),j+1,slot.hours);

				currentChunk += (currentStr + line);
				if (column == NUMBER_OF_COLUMNS-1)
				{
					currentChunk += _T("\n");
					currentLineNumber++;
				}
				else
				{
					if (j+1>=shift.emptySlots.size())
					{
						currentChunk += _T("\n");
						currentLineNumber++;
					}
					else
						currentChunk += _T("   ");
				}
				column++;
				column = column % NUMBER_OF_COLUMNS;
			}
		}
	}
	if (currentLineNumber>=maxNumberOfLines)
	{
		current -= CTimeSpan(1,0,0,0); // back out a day
	}
	else
		chunk += currentChunk;

	return chunk;
}

void NeedsView::GetString(int maxNumberOfLines)
{
	if (!m_output.empty())
		return;
	
	CWaitCursor wait;
	CTime current = GetPeriod().m_dateRange.GetStartDate();
	while (current != GetPeriod().m_dateRange.GetEndDate())
	{
		CString chunk = GetNextChunk(GetPeriod(),current,maxNumberOfLines);
		if (chunk.IsEmpty())
			return;
		m_output.push_back(chunk);
	}
}

void NeedsView::OnDraw(CDC *pDC)
{
	GetString(25);
	if (m_output.empty())
		return;
	CFont arial;
	VERIFY(arial.CreatePointFont(2400,_T("Arial")));
	CFont * oldFont = pDC->SelectObject(&arial);
	CRect rcOut(100,-100,100,-100);
	CRect rcCalc(rcOut);
	if (m_currentPage >= m_output.size())
		m_currentPage = 0;

	pDC->DrawText(m_output[m_currentPage],rcOut,DT_NOCLIP);
	rcOut = CRect(4000,-100,6000,-100);
	CFont * tempFont = pDC->SelectObject(oldFont);
	ASSERT(tempFont == &arial);
}

void NeedsView::OnNextDay()
{
	if ((m_currentPage+1) < m_output.size())
	{
		m_currentPage++;
	}
	
	CString windowTitle;
	windowTitle.Format(_T("Needs View: Page %d"),m_currentPage+1);
	SetWindowText(windowTitle);
	Invalidate();
}

void NeedsView::OnUpdateNextDay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if ((m_currentPage+1) >= m_output.size())
		pCmdUI->Enable(FALSE);
}

void NeedsView::OnPreviousDay()
{
	if (m_currentPage != 0)
		m_currentPage--;
	CString windowTitle;
	windowTitle.Format(_T("Needs View: Page %d"),m_currentPage+1);
	SetWindowText(windowTitle);
	Invalidate();
}

void NeedsView::OnUpdatePreviousDay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_currentPage != 0);

}

void NeedsView::OnPrintall()
{
	CPrintDialog dlg(FALSE);
	if (IDOK == dlg.DoModal())
	{
		HDC dc = dlg.GetPrinterDC();

		CDC DC;
		DC.Attach(dc);
		
		DEVMODE *myMode = dlg.GetDevMode();//fills myMode with printer defaults 
		myMode->dmOrientation = DMORIENT_PORTRAIT;//change default to landscape
		myMode->dmPrintQuality = DMRES_DRAFT;
		myMode->dmColor = DMCOLOR_MONOCHROME;
		DC.ResetDC(myMode);

		DC.m_bPrinting = TRUE;
		OnPrepareDC(&DC);

		DC.StartDoc(_T("NeedsView"));
		for (m_currentPage = 0;m_currentPage<m_output.size();m_currentPage++)
		{
			DC.StartPage();
			OnDraw(&DC);
			DC.EndPage();
		}
		m_currentPage = 0;
		DC.EndDoc();
	}
}
