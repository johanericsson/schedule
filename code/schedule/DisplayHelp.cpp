#include "stdafx.h"
#include "DisplayHelp.h"

void EraseBackground(CDC * dc,CWnd * pWnd)
{
	CRect rc;
	pWnd->GetClientRect(rc);
	dc->FillRect(rc,&CBrush(RGB(255,255,255)));
}