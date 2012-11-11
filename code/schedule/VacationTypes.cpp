#include "stdafx.h"
#include "VacationTypes.h"
#include "XMLNode.h"
#include "vectorhelper.h"


void operator<<(XMLNode& node,const VacationType& t)
{
	SerializeAttribute(node,_T("Code"),t.m_code);
	SerializeAttribute(node,_T("Description"),t.m_description);
}
void operator>>(const XMLNode& node,VacationType& t)
{
	SerializeAttribute(node,_T("Code"),t.m_code);
	SerializeAttribute(node,_T("Description"),t.m_description);
}

CString ToString(const VacationType& t)
{
	return t.m_code + CString(": ") + t.m_description;
}

void operator<<(XMLNode& node,const VacationTypes& t)
{
	Serialize(node,_T("VacationTypeCollection"),t.m_types);
}
void operator>>(const XMLNode& node,VacationTypes& t)
{
	Serialize(node,_T("VacationTypeCollection"),t.m_types);
	sort(t.m_types.begin(),t.m_types.end());
}