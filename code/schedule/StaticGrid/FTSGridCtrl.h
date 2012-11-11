#ifndef _FTSGridCtrl_H
#define _FTSGridCtrl_H

#include <vector>
class CFTSGridImpl;
class CFTSGridCtrl
{   
public:
    CFTSGridCtrl(const CRect& rcCreate,CWnd* pParent,UINT nID,
      	const std::vector<std::vector<CString> >& ContentText,
        const std::vector<std::vector<double> >& Contents,
		const std::vector<bool>& editable);

    ~CFTSGridCtrl();

	const std::vector<std::vector<CString> >& GetStrings();
	void SetStrings(const std::vector<std::vector<CString> >& strings);


    void GetCurrentCell(int& nRow,int& nCol) const;
	void SetCurrentCell(int nRow,int nColl);

    void SetValues(const std::vector<std::vector<double> >&);
    const std::vector<std::vector<double> >& GetValues();    
private:
    CFTSGridCtrl& operator=(const CFTSGridCtrl&);
    CFTSGridCtrl(const CFTSGridCtrl&);

    CFTSGridImpl * m_pImpl;
};

#endif