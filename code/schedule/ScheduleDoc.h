// ScheduleDoc.h : interface of the CScheduleDoc class
//

#pragma once
#include <memory>
#include "NameDlg.H"
#include "name.h"
class CScheduleDoc : public CDocument
{
protected: // create from serialization only
	CScheduleDoc();
	DECLARE_DYNCREATE(CScheduleDoc)
	//Names m_names;
// Attributes
public:
	//auto_ptr<NameDlg> m_pNameDlg;
// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CScheduleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewNamelist();
};


