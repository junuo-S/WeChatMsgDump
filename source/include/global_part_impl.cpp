#include "global_interface.h"

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

STDMETHODIMP_(EventType) IJCoreEvent::Type()
{
	return m_type;
}

JDecryptEvent::JDecryptEvent(const QString& inputFile, const QString& outputFile, const bool bSuc, int totalCount)
	: m_inputFile(inputFile)
	, m_outputFile(outputFile)
	, m_bSuc(bSuc)
	, m_totalCount(totalCount)
{

}

STDMETHODIMP_(EventType) JDecryptEvent::Type()
{
	return EventType::Event_Decrypt;
}

JCombineEvent::JCombineEvent(const QString& currentFile, const QString& finalFile, const bool bSuc, int totalCount)
	: m_currentFile(currentFile)
	, m_finalFile(finalFile)
	, m_bSuc(bSuc)
	, m_totalCount(totalCount)
{

}

STDMETHODIMP_(EventType) JCombineEvent::Type()
{
	return EventType::Event_Combine;
}
