#pragma once


class SignupShift
{
public:
	SignupShift(int startTime,int numberOfPositions):
	  startTime(startTime),
	  numberOfPositions(numberOfPositions)
	  {
	  }
	int startTime;
	int numberOfPositions;
	friend bool operator<(const SignupShift& s1,
					 const SignupShift& s2)
	{
		return (s1.startTime< s2.startTime);
	}
};

class SignupShifts
{
public:
	void Remove(int i)
	{
		m_shifts.erase(m_shifts.begin()+ i);

	}
	void Add(const SignupShift& signupShift)
	{
		m_shifts.push_back(signupShift);
		sort(m_shifts.begin(),m_shifts.end());
	}
	SignupShift& operator[](int i)
	{
		return m_shifts[i];
	}
	const SignupShift& operator[](int i) const
	{
		return m_shifts[i];
	}
private:
	vector<SignupShift> m_shifts;

};