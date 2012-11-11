#include "stdafx.h"
#include "updater.h"
#include <algorithm>
#include <functional>

vector<UpdateSubject*> g_subject;
void UpdateAll()
{
	for_each(g_subject.begin(),g_subject.end(),
		mem_fun(&UpdateSubject::UpdateTheSubject));
}


void Subscribe(UpdateSubject& s)
{
	g_subject.push_back(&s);
}
void SubscribeFirst(UpdateSubject& s)
{
	g_subject.insert(g_subject.begin(),&s);
}
void Unsubscribe(UpdateSubject& s)
{
	vector<UpdateSubject*>::iterator i =
		find(g_subject.begin(),g_subject.end(),
		     &s);
	ASSERT(i!=g_subject.end());
	g_subject.erase(i);
}