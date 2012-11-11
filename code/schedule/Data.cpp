#include "stdafx.h"
#include "data.h"
#include "XMLNode.h"
void operator<<(XMLNode& node,const Data& d)
{
	Serialize(node,_T("ShiftsTemplate"),d.m_ShiftsTemplate);
	Serialize(node,_T("Nurses"),d.m_nurses);
	Serialize(node,_T("ShiftLegend"),d.m_legend);
	Serialize(node,_T("ScheduleAssignments"),d.m_assignments);
	Serialize(node,_T("VacationTypes"),d.m_vacationTypes,VacationTypes());
}
void operator>>(const XMLNode& node,Data& d)
{
	Serialize(node,_T("ShiftsTemplate"),d.m_ShiftsTemplate);
	Serialize(node,_T("Nurses"),d.m_nurses);
	Serialize(node,_T("ShiftLegend"),d.m_legend);
	Serialize(node,_T("ScheduleAssignments"),d.m_assignments);
	Serialize(node,_T("VacationTypes"),d.m_vacationTypes,VacationTypes());
}