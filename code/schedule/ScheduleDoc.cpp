// ScheduleDoc.cpp : implementation of the CScheduleDoc class
//

#include "stdafx.h"
#include "Schedule.h"

#include "ScheduleDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScheduleDoc

IMPLEMENT_DYNCREATE(CScheduleDoc, CDocument)

BEGIN_MESSAGE_MAP(CScheduleDoc, CDocument)
	ON_COMMAND(ID_VIEW_NAMELIST, OnViewNamelist)
END_MESSAGE_MAP()


// CScheduleDoc construction/destruction

CScheduleDoc::CScheduleDoc()
{
	// TODO: add one-time construction code here

}

CScheduleDoc::~CScheduleDoc()
{
}

BOOL CScheduleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CScheduleDoc serialization

void CScheduleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CScheduleDoc diagnostics

#ifdef _DEBUG
void CScheduleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScheduleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CScheduleDoc commands

void CScheduleDoc::OnViewNamelist()
{
}
