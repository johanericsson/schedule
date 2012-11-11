#pragma once
#include "name.h"
#include "ordering.h"
#include "vectorhelper.h"

class Period;
vector<CString> GetAllFixedScheduleStatus(const Period& period,int day);

// Allows you to enable or disable a specific fixed schedule
void SetFixedScheduleStatus(Period& period,int day,
							int number,bool enable);
bool GetFixedScheduleStatus(const Period& period,int day,int number);

class Nurse;
class FixedScheduleOverride;
template <class T>
class Optional;
struct Slot
{
	Slot(Name name,int startTime,int hours,bool fixed):
		name(name),
		startTime(startTime),
		hours(hours),
		fixed(fixed),
		extraWorker(false)
	{
	}
	Slot(){}
	Name name;
	int startTime;
	int hours;
	bool fixed;

	bool extraWorker;

	friend bool operator<(Slot a1,Slot a2)
	{
		COMPARE(startTime);
		COMPARE(hours);
		COMPARE(fixed);
		COMPARE(name);
		COMPARE(extraWorker);
		return false;
	}
};

Optional<Slot> GetWorkingNurse(const Period& period,
							   CTime dayTime,
							   const Nurse& nurse,
							   vector<Slot>&
							   fixedSchedules);

// Given a period, a day, and nurse this returns what the notation is for
// that nurse on that day
CString GetWorkingNotation(const Period& period,
						   CTime day,
						   const Nurse& nurse,
						   vector<Slot>&
						   fixedScheduleOverrides = 
						   vector<Slot>());

Optional<Nurse> GetNurse(const Period& period,const Name& name);


vector<Slot> GetAvailableNurses(const Period& period,CTime dayTime);
vector<Slot> GetWorkingNurses(const Period&, CTime dayTime);

struct EmptySlot
{
	EmptySlot(int hours):hours(hours){}
	int hours;
	friend bool operator<(EmptySlot a1,EmptySlot a2)
	{
		COMPARE(hours);
		return false;
	}
};

struct ShiftSetup
{
	ShiftSetup(int time):
		time(time)
		{
		}
	sorted_vector<Slot> workers;
	int time;
	sorted_vector<EmptySlot> emptySlots;
	int NumberOfSpotsNeeded() const {return workers.size();}
	friend bool operator==(ShiftSetup s1,ShiftSetup s2)
	{
		if (s1.time == s2.time)
			return true;
		return false;
	}

	friend bool operator<(ShiftSetup a1,ShiftSetup a2)
	{
		COMPARE(time);
		COMPARE(workers.size());
		return false;
	}
};

vector<ShiftSetup> GetShifts(const Period& period,CTime day);
vector<ShiftSetup> GetShifts(const Period& period,CTime date,
							 int startCutoff,int stopCutoff);

int GetNumberOfHoursPerWeek(const Period& period,const Nurse& nurse,
					   int weekNumber,vector<vector<Slot> >&
					   cached);

vector<int> GetNumberOfHoursPerWeek(const Period& period,
									const std::vector<Nurse>& nurses,
									int weekNumber);

vector<CString> GetWorkersAtStartTime(const vector<ShiftSetup>& setups,
									  CTime day,
									int startTime,int stopTime);

vector<CString> GetWorkersAtStartTime(const vector<ShiftSetup>& setups,
									  CTime day,
									int startTime);
