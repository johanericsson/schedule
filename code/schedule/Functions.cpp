#include "stdafx.h"
#include "functions.h"
#include "period.h"
#include "optional.H"
#include "updater.h"
#include <functional>
#include <iterator>

enum FixedCategories
{
	SHOW_ONLY_FIXED, 
	SHOW_ONLY_AVAILABLE,
	SHOW_ALL
};

static 
vector<FixedScheduleOverride>
GetOverrides(const Period& period,int day)
{

	return 
		period.m_data.m_assignments[day].m_overrides;
}

static vector<Slot> 
GetNonOverridesReal(const Period& period,int day,FixedCategories showAll)
{
		// First get the set of overrides
	vector<Slot> non_override;

	// for each nurse
	for (int i=0;i<period.m_data.m_nurses.Size();i++)
	{
		const Nurse& nurse =period.m_data.m_nurses[i];
		if (showAll == SHOW_ONLY_FIXED)
		{
			if (nurse.FixedSchedules().IsOnlyAvailable())
				continue;
		}
		if (showAll == SHOW_ONLY_AVAILABLE)
		{
			if (nurse.FixedSchedules().IsOnlyAvailable()==false)
				continue;
		}
		// for each fixed schedule entry
		for (int j=0;j<nurse.FixedSchedules().Size();j++)
		{
			const FixedScheduleEntry& entry = 
				nurse.FixedSchedules()[j];
			CTime today = period.m_dateRange.GetStartDate() + 
				CTimeSpan(day,0,0,0);
			if (nurse.FixedSchedules().IsWorkingOn(today,j))
			{
				Slot newNonOverride(nurse.GetName(),entry.StartTime(),
					entry.Hours(),true);
				const vector<FixedScheduleOverride>&
					overrides = GetOverrides(period,day);
				
				bool bFound = false;
				for (int k=0;k<overrides.size();k++)
				{
					if (overrides[k].m_nurse ==
						newNonOverride.name)
					{
						bFound = true;
						break;
					}
				}
				if (!bFound)
					non_override.push_back(newNonOverride);
			}
		}
	}

	

	return non_override;
}

static vector<Slot> 
GetNonOverrides(const Period& period,int day,FixedCategories  showAll)
{
	return GetNonOverridesReal(period,day,showAll);
}





CString ToString(const FixedScheduleOverride& override)
{
	CString str;
	//str.Format(_T("Start:%d Hours:%d Name:%s"),
		//override.m_fixedSchedule.StartTime(),
		//override.m_fixedSchedule.Hours(),
		//override.m_nurse.FullName());

	return override.m_nurse.FullName();
	return str;
}

vector<CString> GetAllFixedScheduleStatus(
	const Period& period,int day)
{
	vector<CString> titles;
	vector<Slot> non_overrides = GetNonOverrides(period,day,SHOW_ALL);
	for (int i=0;i<non_overrides.size();i++)
	{
		Slot slot = non_overrides[i];
		CString title;
		title.Format(_T(": StartTime:%d Hours:%d"),
			slot.startTime,slot.hours);
		titles.push_back(slot.name.FullName() + title);
	}
	
	const vector<FixedScheduleOverride>& overrides = 
		GetOverrides(period,day);

	for (int i=0;i<overrides.size();i++)
	{
		titles.push_back(CString(_T("Disabled:")) + ToString(overrides[i]));
	}
	return titles;
}

void SetFixedScheduleStatus(Period& period,int day,
							int number,bool enable)
{
	if (!enable)
	{
		vector<Slot> non_overrides = 
			GetNonOverrides(period,day,SHOW_ALL);
		period.m_data.m_assignments[day].AddOverride(non_overrides[number].name);
	}
	else
	{
		vector<Slot> non_overrides = 
			GetNonOverrides(period,day,SHOW_ALL);
		int size = non_overrides.size();
		int remove_override_num = number-size;
		vector<FixedScheduleOverride>& overrides =
			period.m_data.m_assignments[day].
			  m_overrides;
		overrides.erase(overrides.begin() + remove_override_num);
	}
}
bool GetFixedScheduleStatus(const Period& period,int day,int number)
{
	vector<Slot> non_overrides = 
		GetNonOverrides(period,day,SHOW_ALL);
	if (number < non_overrides.size())
		return true;
	return false;
}


Optional<Slot> GetWorkingNurse(const Period& period,
							   CTime dayTime,
							   const Nurse& nurse,
							   vector<Slot>&
							   fixedSchedules)
{
	// Check fixed schedules and check
	// variable schedules. First check fixed schedules

	CTimeSpan span = 
		dayTime-period.m_dateRange.GetStartDate();
	int day = (int)span.GetDays();
	if (fixedSchedules.empty())
		fixedSchedules = GetNonOverrides(period,day,SHOW_ONLY_FIXED);
	for (int i=0;i<fixedSchedules.size();i++)
	{
		if (fixedSchedules[i].name ==
			nurse.GetName())
		{
			return fixedSchedules[i];
		}
	}
	const DailyOverrides&
		daily = period.m_data.m_assignments[day];

	for (int i=0;i<daily.Size();i++)
	{
		const Assignment& assignment = 
			daily[i];
		if (nurse.GetName() == assignment.GetName())
		{
			return Slot(nurse.GetName(),assignment.StartTime(),assignment.NumberOfHours(),false);
		}
	}

	return Optional<Slot>();


}

Optional<VacationSchedule> IsNurseOnVacation(const Period& period,CTime dayTime,
											 const Nurse& nurse)
{
	int day = static_cast<int>((dayTime - period.m_dateRange.GetStartDate()).GetDays());
	const vector<VacationSchedule>& vacations = 
		period.m_data.m_assignments[day].m_vacationSchedules;
	for (int i=0;i<vacations.size();i++)
	{
		if (nurse.GetName() == vacations[i].m_nurse)
			return vacations[i];
	}
	return Empty();
}

CString GetWorkingNotation(const Period& period,
						   CTime dayTime,
						   const Nurse& nurse,
						   vector<Slot>&
						   fixedSchedules)
{
	Optional<VacationSchedule> vacation = IsNurseOnVacation(period,dayTime,nurse);
	if (vacation)
	{
		return vacation->m_code;
	}

	Optional<Slot> slot = GetWorkingNurse(period,dayTime,nurse,fixedSchedules);
	if (!slot)
		return _T("  ");
	else
		return period.m_data.m_legend.GetAbbreviation(slot->startTime,slot->hours);
}

bool IsNurseAlreadyAssigned(const Period& period,
							CTime dayTime,
							const Nurse& nurse)
{
	// Is she working this day already?
	CString isWorking = 
		GetWorkingNotation(period,dayTime,nurse);
	if (isWorking == _T("  "))
		return false;
	return true;
}

bool LessThan(const Nurse& nurse,const Name& name)
{
	return (nurse.GetName() < name);
}
Optional<Nurse> GetNurse(const Period& period,const Name& name)
{
	const Nurses& nurses = period.m_data.m_nurses;
	vector<Nurse>::const_iterator pNurse =
		lower_bound(nurses.begin(),
		nurses.end(),name,&LessThan);
	if (pNurse == nurses.end())
		return Empty();
	return *pNurse;
}

static 
vector<Slot> GetNursesForADay(const Period& period,CTime dayTime,FixedCategories 
							  category)
{
	CTimeSpan span = dayTime - period.m_dateRange.GetStartDate();
	int day = (int)span.GetDays();
	return GetNonOverrides(period,day,category);
}


vector<Slot> GetAvailableNurses(const Period& period,CTime dayTime)
{	
	return GetNursesForADay(period,dayTime,SHOW_ONLY_AVAILABLE);

}

vector<Slot> GetWorkingNurses(const Period& period, CTime dayTime)
{
	vector<Slot> slots;
	for (int i=0;i<period.m_data.m_nurses.Size();i++)
	{
		
		vector<Slot> cachedData;
		Optional<Slot> slot = GetWorkingNurse(period,dayTime,period.m_data.m_nurses[i],cachedData);
		if (slot)
			slots.push_back(*slot);
	}
	return slots;
}

// The number of shifts are a union between the 


struct IsStartTimeEqual
{
	int startTime;
	
	IsStartTimeEqual(int startTime):
	startTime(startTime)
	{
	}

	bool operator()(const ShiftSetup& setup)const
	{
		if (startTime == setup.time)
			return true;
		return false;
	}
};

struct DoesSlotExactlyMatch
{
	DoesSlotExactlyMatch(int hours):hours(hours){}
	bool operator()(const EmptySlot& slot) const
	{
		if (slot.hours == hours)
			return true;
		return false;
	}
	int hours;
};

struct DoesEmployeeFit
{
	DoesEmployeeFit(int hours):hours(hours){}
	bool operator()(const EmptySlot& slot) const
	{
		if (slot.hours > hours)
			return true;
		return false;
	}
	int hours;
};

vector<ShiftSetup> GetShiftsCollapsed(const Period& period,CTime day)
{
	int weekDayNumber = day.GetDayOfWeek()-1;

	const int numberOfHours = 40;
	int emptySlots[numberOfHours];
	for (int i=0;i<numberOfHours;i++)
	{
		emptySlots[i] = 0;
	}

	const WeekDayTemplate& weekDay = 
		period.m_data.m_ShiftsTemplate[weekDayNumber];
	for (int i=0;i<weekDay.Size();i++)
	{
		const ShiftTemplate& shift = weekDay[i];
		for (int hour = shift.startTime;hour < (shift.startTime + shift.hours);hour++)
		{
			emptySlots[hour]+=shift.nurses;
		}
	}

	vector<Slot> workers = GetWorkingNurses(period,day);
	std::sort(workers.begin(),workers.end());

	sorted_vector<ShiftSetup> setups;
	for (int i=0;i<workers.size();i++)
	{
		Slot& worker = workers[i];
		for (int hour = worker.startTime;hour < (worker.startTime + worker.hours);hour++)
		{
			emptySlots[hour]--;
			if (emptySlots[hour] < 0)
				worker.extraWorker = true;
		}
		vector<ShiftSetup>::iterator iter = 
			std::find_if(setups.begin(),setups.end(),IsStartTimeEqual(worker.startTime));
		if (iter!=setups.end())
		{
			iter->workers.push_back(worker);
		}
		else
		{
			ShiftSetup newSetup(worker.startTime);
			newSetup.workers.push_back(worker);
			setups.push_back(newSetup);
		}
	}
	while (1)
	{
		int * end = emptySlots + numberOfHours;
		int * begin = emptySlots;
		// First find start time
		int * startSlot = std::find_if(begin,end,bind2nd(std::greater<int>(),0));
		if (startSlot == end)
			return setups.Get();

		int startTime = startSlot - begin;
		int * endSlot = std::find_if(startSlot,end,bind2nd(std::equal_to<int>(),0));
		int hours = endSlot - startSlot;
		if (hours > 12)
			hours = 12;

		// Add the new shift to the shift setup
		vector<ShiftSetup>::iterator iter = 
			std::find_if(setups.begin(),setups.end(),IsStartTimeEqual(startTime));
		if (iter!=setups.end())
		{
			iter->emptySlots.push_back(hours);
		}
		else
		{
			ShiftSetup newSetup(startTime);
			newSetup.emptySlots.push_back(hours);
			setups.push_back(newSetup);
		}
		for (int i=startTime;i<(startTime + hours);i++)
		{
			emptySlots[i]--;
		}
	}
}


vector<ShiftSetup> GetShifts(const Period& period,
							 CTime day)
{
	if (false)
		return GetShiftsCollapsed(period,day);
	sorted_vector<ShiftSetup> setups;
	int weekDayNumber = day.GetDayOfWeek()-1;

	const WeekDayTemplate& weekDay = 
		period.m_data.m_ShiftsTemplate[weekDayNumber];
	for (int i=0;i<weekDay.Size();i++)
	{
		const ShiftTemplate& shift = weekDay[i];
		vector<ShiftSetup>::iterator iter = 
			std::find_if(setups.begin(),setups.end(),IsStartTimeEqual(shift.startTime));
		if (iter==setups.end())
		{
			ShiftSetup setup(shift.startTime);
			setup.emptySlots.Add(vector<EmptySlot>(shift.nurses,EmptySlot(shift.hours)));
			setups.push_back(setup);
		}
		else
			iter->emptySlots.Add(vector<EmptySlot>(shift.nurses,EmptySlot(shift.hours)));
	}

	vector<Slot> workers = GetWorkingNurses(period,day);
	std::sort(workers.begin(),workers.end());
	// For each worker add a slot
	for (int i=0;i<workers.size();i++)
	{
		// Find the first slot that fits
		vector<ShiftSetup>::iterator iter = 
			std::find_if(setups.begin(),setups.end(),IsStartTimeEqual(workers[i].startTime));
		
		Slot& worker = workers[i];
		// There are no slots with this as the start time, so create a new slot
		if (iter==setups.end())
		{
			ShiftSetup newSetup(worker.startTime);
			worker.extraWorker = true;
			newSetup.workers.push_back(worker);
			setups.push_back(newSetup);
			continue;
		}
		// The start time matches, now see if one of the empty slots has enough hours
		ShiftSetup& currentShift = *iter;
		vector<EmptySlot>::iterator pEmptySlot= find_if(iter->emptySlots.begin(),iter->emptySlots.end(),
												DoesSlotExactlyMatch(worker.hours));
		// If we find a perfect match, then move the employee into an empty slot
		// and remove the empty slot
		if (pEmptySlot!=iter->emptySlots.end())
		{
			iter->workers.push_back(worker);
			iter->emptySlots.erase(pEmptySlot);
			continue; // move on to the next employee
		}
		vector<EmptySlot>::iterator pPartialSlot = find_if(iter->emptySlots.begin(),iter->emptySlots.end(),
															DoesEmployeeFit(worker.hours));
		// Found a slot that can be partially used. Move employee into this slot and then split the slot
		// in 2
		if (pPartialSlot!=iter->emptySlots.end())
		{
			EmptySlot movingSlot = *pPartialSlot;
			iter->emptySlots.erase(pPartialSlot);
			movingSlot.hours -= worker.hours;
			iter->workers.push_back(worker);
			// Then we need to find if there is a place to put the moved slot
			// Find the first slot that fits
			int newStartTime = worker.startTime + worker.hours;
			vector<ShiftSetup>::iterator pNewStartSetup = 
				std::find_if(setups.begin(),setups.end(),IsStartTimeEqual(newStartTime));
			if (pNewStartSetup != setups.end())
			{
				pNewStartSetup->emptySlots.push_back(movingSlot);
			}
			else
			{
				ShiftSetup newSetup(newStartTime);
				newSetup.emptySlots.push_back(movingSlot);
				setups.push_back(newSetup);
			}
			continue; // move on to next employee
		}
		// Found no slot that can be used. Create a new slot
		worker.extraWorker = true;
		iter->workers.push_back(worker);
	}
	return setups.Get();
}


vector<ShiftSetup> GetShifts(const Period& period,
							 CTime day,int startCutoff,
							 int stopCutoff)
{
	vector<ShiftSetup> setups = GetShifts(period,day);
	for (int i=0;i<setups.size();i++)
	{
		if ((stopCutoff <= setups[i].time) ||
			setups[i].time < startCutoff)
		{
			setups.erase(setups.begin() + i);
			--i; // since this element no longer exists, we need to go back before it
		}
			
	}
	return setups;
}


int GetNumberOfHoursPerWeek(const Period& period,const Nurse& nurse,
					   int weekNumber,vector<vector<Slot> >&
					   cached)
{
	int totalHours = 0;
	// for each day of the week
	for (int i=0;i<7;i++)
	{
		
		int dayNumber = weekNumber * 7 + i;
		CTime day = period.m_dateRange.GetStartDate() + 
			CTimeSpan(dayNumber,0,0,0);
		Optional<VacationSchedule> vacation = IsNurseOnVacation(period,day,nurse);
		if (vacation)
		{
			totalHours += nurse.GetDailyHours();
			continue;
		}
		Optional<Slot> slot =  GetWorkingNurse(period,day,
									nurse,cached[i]);
		if (slot)
			totalHours += slot->hours;
	}
	return totalHours;
}

vector<int> 
GetNumberOfHoursPerWeek(const Period& period,
						const std::vector<Nurse>& nurses,
						int weekNumber)
{
	vector<vector<Slot> > cached(7);
	vector<int> hours;
	for (int i=0;i<nurses.size();i++)
	{
		hours.push_back(GetNumberOfHoursPerWeek(period,nurses[i],weekNumber,cached));
	}
	return hours;
}

vector<CString> GetWorkersAtStartTime(const vector<ShiftSetup>& setups,
									  CTime day,
									int startCutoff,int stopCutoff)
{
	vector<CString> names;
	vector<CString> afterNames;
	for (int i=0;i<setups.size();i++)
	{
		for (int j=0;j<setups[i].workers.size();j++)
		{
			CString name;
			long startTime = setups[i].workers[j].startTime;
			long stopTime = startTime + setups[i].workers[j].hours;
			if (startTime <= startCutoff &&
				startCutoff  < stopTime)
			{
				name = setups[i].workers[j].name.FullName();
				CString length;
				length.Format(_T("( %d hrs)"),stopTime-startTime);
				name += length;
				names.push_back(name);
			}
			else
			{
			if (startTime < stopCutoff &&
				stopCutoff < stopTime)
				{
					CString time;
					time.Format(_T("%d00  "),startTime<25?startTime:startTime-24);
					name = time + setups[i].workers[j].name.FullName();
					CString length;
					length.Format(_T("( %d hrs)"),stopTime-startTime);
					name += length;
					afterNames.push_back(name);
				}
			}
		}
	}
	std::copy(afterNames.begin(),afterNames.end(),std::back_inserter(names));
	return names;
}

vector<CString> GetWorkersAtStartTime(const vector<ShiftSetup>& setups,
									  CTime day,
									int startCutoff)
{
	return GetWorkersAtStartTime(setups,day,startCutoff,startCutoff + 4);
}