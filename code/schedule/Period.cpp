#include "stdafx.h"
#include "period.h"
#include <algorithm>
#include "vectorhelper.h"
#include "version.h"
#include "schedule.h"


bool ExistsIn(const Name& name,const std::vector<Name>& names)
{
	for (std::vector<Name>::const_iterator i = names.begin();
		i!=names.end();
		++i)
	{
		if (*i == name)
			return true;
	}
	return false;
}

void NameUpdate(Period& period)
{
		std::vector<Name> availableNames;
	for (int i=0;i<period.m_data.m_nurses.Size();i++)
	{
		availableNames.push_back(period.m_data.m_nurses[i].GetName());
	}

	for (int i=0;i<period.m_data.m_assignments.m_daily.size();i++)
	{
		DailyOverrides& overrides = 
			period.m_data.m_assignments.m_daily[i];
		for (int j=0;j<overrides.Size();j++)
		{
			if (!ExistsIn(overrides[j].GetName(),availableNames))
			{
				overrides.Remove(j);
				--j;
			}
		}
		for (int j=0;j<overrides.m_overrides.size();j++)
		{
			if (!ExistsIn(overrides.m_overrides[j].m_nurse,
				availableNames))
			{
				overrides.m_overrides.erase(
					overrides.m_overrides.begin() + j);
				--j;
			}
		}
	}
}

void Period::Import(const Period& oldPeriod)
{
	m_data.m_legend = oldPeriod.m_data.m_legend;
	m_data.m_nurses = oldPeriod.m_data.m_nurses;
	m_data.m_ShiftsTemplate = oldPeriod.m_data.m_ShiftsTemplate;
	m_data.m_vacationTypes = oldPeriod.m_data.m_vacationTypes;
}

void operator<<(XMLNode& node, const Period& p)
{
	Serialize(node,_T("DateRange"),p.m_dateRange);
	Serialize(node,_T("Data"),p.m_data);
}
void operator>>(const XMLNode& node,Period& p)
{
	Serialize(node,_T("DateRange"),p.m_dateRange);
	Serialize(node,_T("Data"),p.m_data);
	NameUpdate(p);
}


static bool Compare(const Period& first,
					const Period& second)
{
	return (first.m_dateRange.GetStartDate() 
		  > second.m_dateRange.GetStartDate());
}
void Periods::Add(const Period& p)
{
	m_periods.push_back(p);
	sort(m_periods.begin(),m_periods.end(),&Compare);
}

Period& 
Periods::GetCurrentPeriod()
{
	ASSERT(m_current != -1);
	return (*this)[m_current];
}



Periods::Periods():
m_current(-1)
{
}



void NameUpdate()
{
	Period& period =((CScheduleApp*)AfxGetApp())->m_periods.GetCurrentPeriod();
	NameUpdate(period);

}
void operator<<(XMLNode& node, const Periods& p)
{
	node.AddAttribute(_T("Current"),p.m_current);
	XMLNode newNode(_T("PeriodCollection"));
	newNode << p.m_periods;
	node.AddNode(newNode);
}
void operator>>(const XMLNode& node,Periods& p)
{
	const XMLNode& periodCollection = node.GetChild(_T("PeriodCollection"));
	periodCollection >> p.m_periods;
}