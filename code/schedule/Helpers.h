#pragma once

inline CRect GetRect(UINT nResourceID,CWnd* parent)
{
	CWnd * pWnd = parent->GetDlgItem(nResourceID);
	CRect fullRect;
	pWnd->GetWindowRect(&fullRect);
	parent->ScreenToClient(&fullRect);

	return fullRect;
}