#include "stdafx.h"
#include "FTSGridCtrl.h"
#include "FTSGridImpl.h"


CFTSGridCtrl::CFTSGridCtrl(const CRect& rcCreate,CWnd* pParent,UINT nID,
		const std::vector<std::vector<CString> >& TextContent,
        const std::vector<std::vector<double> >& Contents,
		const std::vector<bool>& editable)
{
    m_pImpl = new CFTSGridImpl(rcCreate,pParent,nID,TextContent,Contents,
		editable);
}

CFTSGridCtrl::~CFTSGridCtrl()
{
    delete m_pImpl;
}

const std::vector<std::vector<double> >& CFTSGridCtrl::GetValues()
{
    return m_pImpl->GetValues();
}

const std::vector<std::vector<CString> >& CFTSGridCtrl::GetStrings()
{
	return m_pImpl->GetTextContent();
}


void CFTSGridCtrl::SetValues(const std::vector<std::vector<double> >& Data)
{
    m_pImpl->SetValues(Data);
}

void CFTSGridCtrl::GetCurrentCell(int& nRow,int& nCol) const
{
    m_pImpl->GetCurrentCell(nRow,nCol);
}

void CFTSGridCtrl::SetCurrentCell(int nRow,int nColl)
{
	m_pImpl->SetCurrentCell(nRow,nColl);
}