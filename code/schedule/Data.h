#pragma once
#include "Nurse.h"
#include "Shifts.h"
#include "ShiftLegend.h"
#include "ScheduleAssignments.h"
#include "VacationTypes.h"

struct Data
{
	ShiftsTemplate m_ShiftsTemplate;
	Nurses m_nurses;
	ShiftLegend m_legend;
	ScheduleAssignments m_assignments;
	VacationTypes m_vacationTypes;
};
class XMLNode;
void operator<<(XMLNode& node,const Data&);	
void operator>>(const XMLNode& node,Data&);	