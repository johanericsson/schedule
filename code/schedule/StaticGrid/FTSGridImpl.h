#ifndef _FTSGridImpl_H
#define _FTSGridImpl_H
#include <vector>
#include "GridCtrl.h"
class CFTSGridImpl: public CGridCtrl
{   
public:
    CFTSGridImpl(const CRect& rcCreate,CWnd* pParent,UINT nID,
		const std::vector<std::vector<CString> >& TextContent,
        const std::vector<std::vector<double> >& Contents,
		const std::vector<bool>& editable);

	const std::vector<std::vector<CString> >& GetTextContent();

    const std::vector<std::vector<double> >& GetValues();
    void SetValues(const std::vector<std::vector<double> >&);
    void GetCurrentCell(int& nRow,int& nCol) const;
	void SetCurrentCell(int nRow,int nCol);


    void SetStringEditable(int index,bool bEditable){m_stringEditable[index] = bEditable;}
private:
    
    void SetupText();
    static bool StringToDouble(CString& string,double& dOut);
    static CString DoubleToString(double dIn);
    BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject);
    void OnEditCell(int nRow,int nCol,CPoint point,UINT nChar);
    void OnEndEditCell(int nRow, int nCol, CString str);
    void ValidateData();

    bool IsStringCell(int nRow,int nCol);
    bool IsEditable(int nRow,int nCol);


	int ColumnOffset() const
	{
		return m_textContents[0].size();
	}
	std::vector<bool> m_stringEditable;
	std::vector<std::vector<CString> > m_textContents;

    std::vector<std::vector<double> >m_Contents;
    CString m_strBuf;
    
};

#endif