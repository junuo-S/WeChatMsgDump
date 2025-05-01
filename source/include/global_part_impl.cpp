#include "global_interface.h"

ULONG STDMETHODCALLTYPE IJUnknown::AddRef()
{
	return ++m_ref;
}

ULONG STDMETHODCALLTYPE IJUnknown::Release()
{
	ULONG ref = --m_ref;
	if (ref == 0) delete this;
	return ref;
}

STDMETHODIMP_(void) IJCoreSubject::AttachObserver(IJCoreObserver* observer)
{
	if (!observer || std::find(m_observers.begin(), m_observers.end(), observer) != m_observers.end())
		return;
	m_observers.push_back(observer);
}

STDMETHODIMP_(void) IJCoreSubject::DetachObserver(IJCoreObserver* observer)
{
	auto iter = std::find(m_observers.begin(), m_observers.end(), observer);
	if (iter != m_observers.end())
		m_observers.erase(iter);
}

STDMETHODIMP_(void) IJCoreSubject::Notify(IJCoreEvent* event)
{
	for (IJCoreObserver* observer : m_observers)
	{
		if (observer && observer->OnCoreEvent(event))
			break;
	}
}

IJCoreObserver::~IJCoreObserver()
{
	for (IJCoreSubject* subject : m_subjects)
	{
		subject->DetachObserver(this);
	}
	m_subjects.clear();
}

void IJCoreObserver::attachTo(IJCoreSubject* subject)
{
	if (!subject || std::find(m_subjects.begin(), m_subjects.end(), subject) != m_subjects.end())
		return;
	subject->AttachObserver(this);
	m_subjects.push_back(subject);
}

void IJCoreObserver::detachFrom(IJCoreSubject* subject)
{
	auto iter = std::find(m_subjects.begin(), m_subjects.end(), subject);
	if (iter == m_subjects.end())
		return;
	subject->DetachObserver(this);
	m_subjects.erase(iter);
}
