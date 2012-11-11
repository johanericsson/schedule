// FTSGridCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "FTSGridImpl.h"
#include <vector>
#include "GridCtrl.h"

CFTSGridImpl::CFTSGridImpl(const CRect& rcCreate,CWnd* pParent,UINT nID,
		const std::vector<std::vector<CString> >& textContent,
        const std::vector<std::vector<double> >& Contents,
		const std::vector<bool>& editable):
CGridCtrl(textContent.size(),textContent[0].size() + Contents[0].size()),
m_Contents(Contents),
m_textContents(textContent),
m_stringEditable(editable)
{
    ASSERT(m_textContents.size() == m_Contents.size());
	ASSERT(editable.size() == ColumnOffset());
    Create(rcCreate,pParent,nID);
    SetupText();
    
    const int iFirst = 100;
    SetColumnWidth(0,iFirst);
    int iColumnWidth = (rcCreate.Width()-iFirst)/(GetColumnCount()-1)
                            -1;

    
	iColumnWidth -=5;
    int i;
    for (i=1;i<GetColumnCount();i++)
        SetColumnWidth(i,iColumnWidth);
}

const std::vector<std::vector<double> >& CFTSGridImpl::GetValues()
{
    EndEditing();
    return m_Contents;
}

const std::vector<std::vector<CString> >& CFTSGridImpl::GetTextContent()
{
    EndEditing();
    return m_textContents;
}



void CFTSGridImpl::SetValues(const std::vector<std::vector<double> >& Data)
{
    ASSERT(Data.size() == m_Contents.size());
    ASSERT(Data[0].size() == m_Contents[0].size());

    m_Contents = Data;
    SetupText();
}
bool CFTSGridImpl::StringToDouble(CString& string,double& dOut)
{
    if (0==_stscanf(string.GetBuffer(0),_T("%lf"),&dOut))
        return false;
    return true;

}

CString CFTSGridImpl::DoubleToString(double dIn)
{
    CString str;
    str.Format(_T("%.5g"),dIn);
    return str;
}

void CFTSGridImpl::SetupText()
{
    // First initialize the text
    int i,j;

	for (i=0;i<GetRowCount();i++)
		for (j=0;j<m_textContents[i].size();j++)
		{
			SetItemText(i,j,m_textContents[i][j]);
		}

	for (i=0;i<GetRowCount();i++)
		for (j=0;j<m_Contents[0].size();j++)
		{
			const double dVal = m_Contents[i][j];
			const CString str = DoubleToString(dVal);
			SetItemText(i,j+ColumnOffset(),str);
		}
	
    Invalidate();
}
    
void CFTSGridImpl::ValidateData()
{
    int i,j;

	for (i=0;i<GetRowCount();i++)
		for (j=0;j<m_Contents[0].size();j++)
		{
			double& dCur = m_Contents[i][j];
			CString str = GetItemText(i,ColumnOffset() + j);
			StringToDouble(str,dCur);
		}
    SetupText(); 
}


bool CFTSGridImpl::IsStringCell(int nRow,int nCol)
{
    if (nCol < ColumnOffset())
        return true;
    
    return false;
}

bool CFTSGridImpl::IsEditable(int nRow,int nCol)
{
    if (IsStringCell(nRow,nCol))
	{
		return m_stringEditable[nCol];
	}
    return true;
}


void CFTSGridImpl::OnEditCell(int nRow,int nCol,CPoint point,UINT nChar)
{
    if (!IsEditable(nRow,nCol))
        return;
    
    m_strBuf = GetItemText(nRow,nCol);
    CGridCtrl::OnEditCell(nRow,nCol,point,nChar);
}




void CFTSGridImpl::OnEndEditCell(int nRow, int nCol, CString str)
{
    ASSERT(IsEditable(nRow,nCol));
    

    CGridCtrl::OnEndEditCell(nRow,nCol,str);
    double dValue;
    CString strEntry = GetItemText(nRow,nCol);
    
    if (IsStringCell(nRow,nCol))
    {
		m_textContents[nRow][nCol] = strEntry;
    }
    else
    if (!StringToDouble(strEntry,dValue))
    {    
        AfxMessageBox(_T("Invalid entry"));
        SetItemText(nRow,nCol,m_strBuf);
    }
    else
    {
        SetItemText(nRow,nCol,DoubleToString(dValue));
		m_Contents[nRow][nCol - ColumnOffset()] = dValue;
    }
}
   
BOOL CFTSGridImpl::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject)
{
    if (CGridCtrl::PasteTextToGrid(cell,pDataObject))
    {
        ValidateData();
        return TRUE;
    }
    return FALSE;

}


void CFTSGridImpl::GetCurrentCell(int& nRow,int& nCol) const
{
    CCellID id = GetFocusCell();
    nRow = id.row;
    nCol = id.col;
}

void CFTSGridImpl::SetCurrentCell(int nRow,int nCol)
{
	CCellID id;
	id.row = nRow;
	id.col = nCol;
	SetFocusCell(id);
}