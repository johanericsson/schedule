#pragma once


void UpdateAll();
struct UpdateSubject;
void Subscribe(UpdateSubject& s);
void SubscribeFirst(UpdateSubject& s);
void Unsubscribe(UpdateSubject& s);

void NameUpdate();
struct UpdateSubject
{
	UpdateSubject()
	{ 
		Subscribe(*this);
	}
	~UpdateSubject()
	{
		Unsubscribe(*this);
	}
	virtual void UpdateTheSubject() = 0;
};

